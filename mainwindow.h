#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <qboxlayout.h>
#include <QLineEdit>

#include "Keyboard.hpp"



class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Keyboard *k_ = nullptr;
    QLineEdit *line_ = nullptr;
public:
    MainWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        auto layout = new QVBoxLayout(this);

        Keyboard *k_ = new Keyboard(0x0);
        connect(k_, SIGNAL(keyPressed(QString)), this, SLOT(oneKeyPressed(QString)));
        connect(k_, SIGNAL(backspacePressed()), this, SLOT(backspacePressed()));
        connect(k_, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

        line_ = new QLineEdit;
        layout->addWidget(line_);
        layout->addWidget(k_);


        setLayout(layout);


        QSize size(k_->getSize().width(), k_->getSize().height() + line_->height());
        resize(size);
    }



public slots:
    void oneKeyPressed(QString t) {
        qDebug() << " keyPressed " << t;
        line_->insert(t);
    }

    void returnPressed( ) {
        qDebug() << " return ";
    }

    void backspacePressed( ) {
        qDebug() << " backspace ";
        line_->backspace();
    }
};

#endif // MAINWINDOW_H
