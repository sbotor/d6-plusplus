#include <fstream>
#include <vector>
#include <thread>
#include <functional>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include "QKeyEvent"
#include "filewindow.h"

#include "Core\Converter.h"
#include "Core\Evaluator.h"
#include "Utility\History.h"
#include "Utility\UtilityFunctions.h"
#include "Core\MathFunc.h"
#include "Utility\CircBuff.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , historyItemsStep(5)
{
    ui->setupUi(this);

    historyCount = 0;
    maxHistoryItems = historyItemsStep;
    std::thread init (&MainWindow::readHistory, this);

    ui->InputLine->installEventFilter(this);

    init.join();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) // TODO
{
    if (obj == ui->InputLine)
    {
        if (event->type() == QEvent::KeyPress && !commands.empty()) // Check if there are any commands
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Up)
            {
                if (commands.isReset())
                    commands.set();
                else
                    commands.next();

                fetchCommand();
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Down)
            {
                if (commands.isReset()) // Do nothing if at the beginning
                    return true;

                commands.prev();
                fetchCommand();
                return true;
            }

            return QMainWindow::eventFilter(obj, event);
        }
        return false;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::readHistory()
{
    ui->ResultHistory->clear(); // Clear previous history

    std::fstream inpFile;
    // Create the file if it does not exist
    inpFile.open(historyFilename, std::ios::out | std::ios::app);
    inpFile.close();

    inpFile.open(historyFilename, std::ios::in);
    if (inpFile.is_open()) // Read the last lines using a circular buffer
    {
        historyCount = 0;
        std::string inpString;
        CircBuff<std::string> buffer(maxHistoryItems);
        while(std::getline(inpFile, inpString, '\n'))
        {
            buffer.push(inpString);
            ++historyCount;
        }
        inpFile.close();

        while (!buffer.empty())
        {
            if (!buffer.front().empty())
            {
                History h(buffer.front());
                addHistory(h);
            }
            buffer.pop();
        }
    }
    else
        throw std::ios::failure("Cannot open history file: \"" + historyFilename + "\"");
}

void MainWindow::saveHistory(const History &h)
{
    std::ofstream outFile(historyFilename, std::ios::app);
    if (outFile.is_open())
    {
        outFile << h;
        outFile.close();
        historyCount++;
    }
    else
        throw std::ios::failure("Cannot open history file: \"" + historyFilename + "\"");
}

void MainWindow::extendHistory(const size_t& n)
{
    maxHistoryItems = n;
}

void MainWindow::fetchCommand()
{
    if (!commands.empty())
        ui->InputLine->setText(QString::fromStdString(commands.read()));
}

QTreeWidgetItem* MainWindow::addHistory(const History& h)
{
    QTreeWidgetItem* parent = new QTreeWidgetItem();
    parent->setText(1, QString::fromStdString(h.getResult()));
    parent->setText(0, QString::fromStdString(h.getExpression()));

    QTreeWidgetItem* child = new QTreeWidgetItem(parent);
    child->setText(0, QString::fromStdString("Details:"));
    child->setText(1, QString::fromStdString(h.getDetails()));

    ui->ResultHistory->insertTopLevelItem(0, parent);
    ui->ResultHistory->setCurrentItem(parent);

    return parent;
}

void MainWindow::on_InputLine_returnPressed()
{
    emit ui->CalculateButton->clicked();
}

void MainWindow::on_CalculateButton_clicked()
{
    QString inputText = ui->InputLine->text();
    if (inputText.length() != 0)
    {
        std::string inp = inputText.toStdString(); // Get text

        Converter conv(inp); // Convert
        if (conv()) // Conversion error
        {
            conv.getError()->prompt(this);
            return;
        }

        Evaluator ev(conv.getConverted()); // Evaluate
        if (ev()) // Evaluation error
        {
            ev.getError()->prompt(this);
            return;
        }

        History h(conv, ev); // Get info
        commands.add(h.getExpression()); // Add to command history

        // Save to file in a different thread
        std::thread saveThread(&MainWindow::saveHistory, this, std::cref(h));

        // Append to history
        if (ui->ResultHistory->topLevelItemCount() == maxHistoryItems) // Delete old item
            delete ui->ResultHistory->takeTopLevelItem(maxHistoryItems - 1);

        emit ui->ResultHistory->itemClicked(addHistory(h), 0);
        ui->InputLine->clear();

        saveThread.join();
    }
}

void MainWindow::on_ResultHistory_itemClicked(QTreeWidgetItem *item, int column)
{
    QTextEdit* hlr_p = ui->HighlightedRes;
    hlr_p->clear();

    QTreeWidgetItem* level1 = nullptr;
    QTreeWidgetItem* level2 = nullptr;

    if (item->parent() == nullptr) // Top level item clicked
    {
        level1 = item;
        level2 = item->child(0);
    }
    else // Second level item clicked
    {
        level1 = item->parent();
        level2 = item;
    }

    hlr_p->setAlignment(Qt::AlignCenter);

    // Write result
    hlr_p->setFontItalic(false);
    hlr_p->setFontPointSize(12);
    hlr_p->insertPlainText("Result: ");
    hlr_p->setFontWeight(QFont::Bold);
    hlr_p->insertPlainText(level1->text(1));

    // Write expression
    hlr_p->setFontPointSize(10);
    hlr_p->setFontWeight(QFont::Normal);
    hlr_p->append(level1->text(0));

    // Write details
    hlr_p->setFontPointSize(10);
    hlr_p->setFontItalic(true);
    hlr_p->append(level2->text(1));
}

void MainWindow::on_ResultHistory_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->parent() == nullptr && column == 0)
    {
        ui->InputLine->setText(item->text(0));
        emit ui->CalculateButton->clicked();
    }
}

void MainWindow::on_ClearHistory_clicked()
{
    QMessageBox::StandardButton button = QMessageBox::information(this, "Clear history", "Do you want to clear the result history?", QMessageBox::Yes, QMessageBox::No);
    if (button == QMessageBox::Yes)
    {
        ui->ResultHistory->clear();
        ui->HighlightedRes->clear();

        std::ofstream outFile(historyFilename, std::ios::trunc);
        if (outFile.is_open())
            outFile.close();
        else
            throw std::ios::failure("Cannot open history file: \"" + historyFilename + "\"");
    }

}

void MainWindow::on_MoreHistory_clicked()
{
    if (historyCount > maxHistoryItems)
    {
        extendHistory(maxHistoryItems + historyItemsStep);
        readHistory();
    }
}

void MainWindow::on_InputLine_textEdited(const QString &arg1)
{
    commands.reset();
}

void MainWindow::on_actionProcess_a_file_triggered()
{
    FileWindow fw;

    fw.setFixedSize(400, 200);
    fw.exec();
}
