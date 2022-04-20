#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QObject>
#include <QString>
#include <QIcon>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyle>
#include <QDebug>


class key : public QObject {
    Q_OBJECT
public:
    explicit key(QString t,QObject *parent = 0)
        : QObject(parent) {
        text = t;
        X =10;
        Y =10;
        W =t.length()*5 + 20;
        H =25;
        pressed = false;
    }

    void setX(int  x) {
        //  qDebug() << "setX " << x << " " << text;
        X = x;
    }

    void setY(int y) {
        //  qDebug() << "setY " << y;
        Y = y;
    }

    void setIconFile(QString i) {
        iconFilename = i;
        W = 25;
    }

    void setPressed( bool b) {
        //   qDebug() << "setPessed " << b;
        pressed = b;
    }

    void draw(QPainter *painter,QStyle *style) {

        QStyleOptionButton opt;
        opt.palette = QPalette(Qt::white);
        /*
           if ( pressed ) {
               opt.state = QStyle::State_Active;
           } else {
                opt.state = QStyle::State_Enabled;
           }
           */
        opt.rect = QRect(X, Y, W, H);

        if ( iconFilename !="" )
        {
            opt.icon = QIcon(iconFilename);
            opt.iconSize=QSize(16,16);
        }
        else
        {
            if (text =="&")  opt.text = "&&";
            else opt.text = text;
        }
        style->drawControl(QStyle::CE_PushButton, &opt, painter);
    }

    QRect getRect()    {
        return QRect(X,Y,W,H);
    }

signals:

public slots:

public :
    int X;
    int Y;
    int W;
    int H;
    QString text;
    QString iconFilename;
    bool pressed;
};




class Keyboard : public QWidget
{
    Q_OBJECT

public:
    Keyboard(QWidget *p);

signals:
    void keyPressed( QString t);
    void backspacePressed(  );
    void returnPressed(  );

private :
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent * E);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent *);

    void initTooltip();
    void initKeys( int indexArraykeys,const char *keymap[]);
    key *findKey(QPoint p);
    void setKeyPressed( key *k,QPoint );

    QVector<QVector< key * > > keys;
    QLabel *tooltip;
    key *currentKey;
    int currentindexkeyboard;
    bool uppercase ;
};

#endif // KEYBOARD_H
