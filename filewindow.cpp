#include "filewindow.h"
#include "ui_filewindow.h"

#include <exception>

#include <QFileDialog>
#include <QMessageBox>
#include <QElapsedTimer>

#include "Utility\UtilityFunctions.h"

FileWindow::FileWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileWindow)
{
    ui->setupUi(this);
    ui->inpFilename->setFocus();
}

FileWindow::~FileWindow()
{
    delete ui;
}

void FileWindow::on_openOutp_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Select the output file", QString());
    if (filename.length() > 0)
        ui->outpFilename->setText(filename);
}

void FileWindow::on_openInp_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select an input file", QString(), "Text files (*.txt *.csv)");
    if (filename.length() > 0)
        ui->inpFilename->setText(filename);
}

void FileWindow::on_okButton_clicked()
{
    if (processFile())
        close();
}

void FileWindow::on_cancelButton_clicked()
{
    close();
}

bool FileWindow::processFile()
{
    QElapsedTimer timer;
    timer.start();
    
    std::string inpFilename = ui->inpFilename->text().toStdString(),
            outpFilename = ui->outpFilename->text().toStdString();
    MassCalc calc;

    // Read and process
    std::ifstream inpFile(inpFilename);
    if (inpFile.is_open())
    {
        calc.process(inpFile);
        inpFile.close();
    }
    else
    {
        QMessageBox::warning(this, "File error", QString::fromStdString("Cannot open input file:\n" + inpFilename));
        return false;
    }

    std::ofstream outpFile(outpFilename);
    if (outpFile.is_open())
    {
        outpFile << "Result; Expression; Details\n";
        calc.save(outpFile);
        outpFile.close();
    }
    else
    {
        QMessageBox::warning(this, "File error", QString::fromStdString("Cannot open output file:\n" + outpFilename));
        return false;
    }

    qDebug() << timer.elapsed();

    QString msg = "Processed " + QString::fromStdString(to_clean_string(calc.size())) +
            " items\nErrors: " + QString::fromStdString(to_clean_string(calc.getErrNum()));
    QMessageBox::information(this, "Done", msg);

    return true;
}
