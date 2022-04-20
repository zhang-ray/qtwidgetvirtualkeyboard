#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDebug>

#include "mainwindow.h"
#include "keyboard.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0)
        : QMainWindow(parent)
    {

        Keyboard *k = new Keyboard(0x0);
        this->setCentralWidget(k);
        connect(k, SIGNAL(keyPressed(QString)), this, SLOT(oneKeyPressed(QString)));
        connect(k, SIGNAL(backspacePressed()), this, SLOT(backspacePressed()));
        connect(k, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

        resize(k->getSize());
    }



public slots:
    void oneKeyPressed(QString t) {
        qDebug() << " keyPressed " << t;
    }

    void returnPressed( ) {
        qDebug() << " return ";
    }

    void backspacePressed( ) {
        qDebug() << " backspace ";
    }
};

#endif // MAINWINDOW_H
