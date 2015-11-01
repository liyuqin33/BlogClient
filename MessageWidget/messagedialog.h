#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QPoint>

#include "message.h"
#include "piclabel.h"


class MessageDialog : public QDialog
{
    Q_OBJECT
public:
    MessageDialog(MESSAGE &msg, QWidget *parent = 0);
    ~MessageDialog();
private:
    void init();
    void initData();
    QHBoxLayout     *horizontalLayout;
    QGridLayout     *gridLayout;
    QHBoxLayout     *up_horizontalLayout;
    QSpacerItem     *horizontalSpacer;
    QPushButton     *closeBtn;
    QVBoxLayout     *left_verticalLayout;
    PicLabel          *headLab;
    QSpacerItem     *verticalSpacer;
    QVBoxLayout     *right_verticalLayout;
    QHBoxLayout     *horizontalLayout_1;
    PicLabel          *nameLab;
    QSpacerItem     *horizontalSpacer_4;
    PicLabel          *contentLab;
    QHBoxLayout     *horizontalLayout_2;
    QFrame          *picWidget;
    QWidget         *up_frame;

    MESSAGE &_msg;

    bool    _isLeftMouseBtnPressed;             //是否按下
    QPoint  _ptLast;                            //按下位置
signals:
protected:
    void mouseMoveEvent(QMouseEvent *event);         //重写鼠标移动事件
    void mousePressEvent(QMouseEvent *event);        //重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *event);     //重写鼠标松开事件
    void paintEvent(QPaintEvent */*event*/);        //重绘
public slots:
    void clickedMoreBtn();

};

#endif // MESSAGEDIALOG_H
