#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QString>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget window;
    window.show();

    return a.exec();
}
