#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    try
    {
        w.show();
        return a.exec();
    }
    catch(const std::ios::failure& e)
    {
        QMessageBox::critical(&w, "Error", QString::fromUtf8(e.what()));
        return 1;
    }
    catch(...)
    {
        QMessageBox::critical(&w, "Error", "An unknown error has occured.");
        return 2;
    }
}
