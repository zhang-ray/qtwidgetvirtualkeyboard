#pragma once

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
#include <QPainter>
#include <QMouseEvent>




namespace {

constexpr int NB_KEYS_TYPES = 4;

constexpr int LOWERCASE =0;
constexpr int NUMBER =1;
constexpr int UPPERCASE =2;
constexpr int PUNCTUATION =3;

const char *en_lower_keymap[] = {
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
    "a", "s", "d", "f", "g", "h", "j", "k", "l",";",
    "Caps", "z", "x", "c", "v", "b", "n", "m",",", "backspace",
    "123", ".", "space", "@", "return",
};

const char *en_upper_keymap[] = {
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L",";",
    "Caps", "Z", "X", "C", "V", "B", "N", "M",",", "backspace",
    "123", ".", "space", "@", "return",
};

const char *en_number_keymap[] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "/", ":", ";", "(", ")", "€", "&", "@", "\"",
    "#+=", ".", ",", "?", "!", "'", "+","\\","%","backspace",
    "ABC", ",", "space", ".", "return",
};

const char *en_punctuation_keymap[] = {
    "[", "]", "{", "}", "#", "%", "^", "*", "+", "=",
    "_", "\\", "|", "~", "<", ">", "=","$", "@", "\"",
    "123", ".", ",", "?", "!", "'","/",":",";", "backspace",
    "ABC", ",", "space", ".", "return",
};

// In witch row are the key... (there's 4 rows )
const int row_keymapp[] = {
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3
};

const int nbkey = sizeof(en_lower_keymap)/ sizeof(char *);

}

class Key : public QObject {
    Q_OBJECT
public:
    constexpr static int BASE_SIZE = 50;

public:
    explicit Key(QString t,QObject *parent = 0)
        : QObject(parent) {
        text = t;
        X =BASE_SIZE;
        Y =BASE_SIZE;
        W =t.length()*BASE_SIZE;
        H =BASE_SIZE;
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
        W = BASE_SIZE;
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
    Keyboard(QWidget *p)
        : QWidget(p) {
        currentKey = 0x0;
        currentindexkeyboard = LOWERCASE;
        uppercase = false;

        initTooltip();

        keys = QVector<QVector< Key * > >(NB_KEYS_TYPES);
        for (int n = 0; n < NB_KEYS_TYPES ; n++)
        {
            keys[n] = QVector< Key * >(nbkey);
        }

        initKeys(LOWERCASE,en_lower_keymap);
        initKeys(NUMBER,en_number_keymap);
        initKeys(UPPERCASE,en_upper_keymap);
        initKeys(PUNCTUATION,en_punctuation_keymap);
    }

public:
    // 获取整个键盘的尺寸
    const QSize getSize() const {
        return {Key::BASE_SIZE * 12, Key::BASE_SIZE * 4};
    }

signals:
    void keyPressed( QString t);
    void backspacePressed(  );
    void returnPressed(  );

private :
    void paintEvent(QPaintEvent*) {
        QPainter painter(this);
        foreach (Key *k, keys[currentindexkeyboard])
        {
            k->draw(&painter,style());
        }
    }
    void mousePressEvent(QMouseEvent * e) {
        QPoint pos = e->pos();
        setKeyPressed( findKey(pos), pos );
    }
    void mouseMoveEvent(QMouseEvent * e) {
        QPoint pos = e->pos();

        if (currentKey != 0x0 && !currentKey->getRect().contains(pos)) {
            tooltip->hide();
            currentKey->setPressed(false);
            this->repaint();
        }
        setKeyPressed( findKey(pos), pos );
    }
    void mouseReleaseEvent(QMouseEvent *e) {
        QPoint pos = e->pos();
        tooltip->hide();
        Key *k= findKey( pos );
        if (k != 0x0 )
        {
            if ( k->text == "ABC")
            {
                currentindexkeyboard = LOWERCASE;
                repaint();
                return;
            }
            if (k->text== "#+=" )
            {
                currentindexkeyboard = PUNCTUATION;
                repaint();
                return;
            }

            if (k->text=="Caps")
            {   if ( uppercase ==false)
                {
                    currentindexkeyboard = UPPERCASE;
                    uppercase = true;
                }
                else
                {
                    currentindexkeyboard = LOWERCASE;
                    uppercase = false;
                }
                repaint();
                return;
            }

            if (k->text=="123")
            {
                currentindexkeyboard = NUMBER;
                repaint();
            }
            else
            {
                if ( k->text =="backspace" )
                {
                    emit backspacePressed();
                    return;
                }

                if (k->text=="enter")
                {
                    emit returnPressed();
                    return;
                }
                if ( k->text == "space")
                {
                    emit keyPressed(" ");
                } else
                {
                    emit keyPressed(k->text);
                }
            }
        }
    }

    void initTooltip() {
        tooltip = new QLabel("");
        tooltip->setWindowFlags( Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

        QFont serifFont("Times", 18, QFont::Bold);
        tooltip->setFont(serifFont);
        tooltip->setAlignment(Qt::AlignCenter);

    }
    void initKeys( int indexArraykeys,const char *keymap[])
    {
        int row = 0;
        for(int n=0; n< nbkey; n++)
        {
            keys[indexArraykeys][n] = new Key(keymap[n]);
            if ( n>0)
            {
                if (strcmp(keymap[n], "return") == 0)     keys[indexArraykeys][n]->setIconFile(":/img/img/enter.png");
                if (strcmp(keymap[n], "backspace") ==0)  keys[indexArraykeys][n]->setIconFile(":/img/img/backspace.jpg");
                if (strcmp(keymap[n], "Caps") == 0)       keys[indexArraykeys][n]->setIconFile(":/img/img/caps.png");
                if (strcmp(keymap[n], "space") == 0)      keys[indexArraykeys][n]->W = Key::BASE_SIZE * 4;

                if (row_keymapp[n-1]!=row_keymapp[n]) {
                    row ++;
                    keys[indexArraykeys][n]->setX(0); //offetrows[row]);
                } else {
                    keys[indexArraykeys][n]->setX(keys[indexArraykeys][n-1]->X + keys[indexArraykeys][n-1]->W);
                }
                keys[indexArraykeys][n]->setY(row_keymapp[n] * Key::BASE_SIZE);
            } else {
                //  keys[indexArraykeys][n]->setX(offetrows[0]);
                keys[indexArraykeys][n]->setX(0);
                keys[indexArraykeys][n]->setY(row_keymapp[n] * Key::BASE_SIZE);
            }
        }
    }
    Key *findKey(QPoint p) {
        foreach (Key *k, keys[currentindexkeyboard]) {
            if (k->getRect().contains(p)) {
                return k;
            }
        }
        return 0x0;
    }
    void setKeyPressed(Key *k, QPoint pos) {
        Q_UNUSED(pos);

        currentKey = k;
        if (k == 0x0) return;

        k->setPressed(true);
        this->repaint();

        QPoint p = QWidget::mapToGlobal(this->pos() +QPoint( k->X, k->Y));
        tooltip->setGeometry(p.x() - Key::BASE_SIZE * 0.25, p.y() - Key::BASE_SIZE * 1.7 , Key::BASE_SIZE * 1.5, Key::BASE_SIZE * 1.5);
        tooltip->setText(k->text);
        tooltip->show();
    }

    QVector<QVector< Key * > > keys;
    QLabel *tooltip;
    Key *currentKey;
    int currentindexkeyboard;
    bool uppercase ;
};


