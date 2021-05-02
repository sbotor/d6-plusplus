#ifndef FILEWINDOW_H
#define FILEWINDOW_H

#include <QDialog>

#include "Core/MassCalc.h"

namespace Ui {
class FileWindow;
}

class FileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileWindow(QWidget *parent = nullptr);
    ~FileWindow();

private slots:
    void on_openOutp_clicked();

    void on_openInp_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::FileWindow *ui;

    bool processFile();
};

#endif // FILEWINDOW_H
