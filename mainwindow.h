#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QTreeWidgetItem"

#include "Utility\History.h"
#include "Utility\CommHist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private slots:
    void on_InputLine_returnPressed();

    void on_CalculateButton_clicked();

    void on_ResultHistory_itemClicked(QTreeWidgetItem *item, int column);

    void on_ResultHistory_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_ClearHistory_clicked();

    void on_MoreHistory_clicked();

    void on_InputLine_textEdited(const QString &arg1);

    void on_actionProcess_a_file_triggered();

private:
    Ui::MainWindow *ui;
    const std::string historyFilename = "history.txt";
    const size_t historyItemsStep;
    size_t maxHistoryItems;
    size_t historyCount;
    CommHist commands;

    void readHistory();
    void saveHistory(const History& h);
    void extendHistory(const size_t& n);
    QTreeWidgetItem* addHistory(const History& h);
    void fetchCommand();
};
#endif // MAINWINDOW_H
