#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "messagedialog.h"
#include "piclabel.h"
#include "message.h"



class ItemWidget : public QWidget
{
    Q_OBJECT
public:
    ItemWidget(MESSAGE msg, QWidget *parent = 0);
    ~ItemWidget();

    void setName(QString name);                     //设置名字
    void setPicHeadName(QString picHeadName);       //设置图片名
    void setMsgStyle(MESSAGE_TYPE msg);             //设置消息类型
    void setDayAndtime(QString day, QString time);  //设置时间
private:
    void initUi();
    void initLayout();
    void initConnect();


    PicLabel *_picLabel;                            //图片label
    QPixmap _pic;                                   //图片pix
    QPushButton *_nameBtn;                          //名字按钮
    QPushButton *_msgStyleBtn;                      //消息类型按钮
    QLabel *_day;                                   //日期
    QLabel *_time;                                  //时间

    MESSAGE _msg;

    QHBoxLayout *_mainLayout;

signals:
    void clickBlogOwner(QString);
    void clickUrl(QString);

public slots:
    void handldClicked1();
    void handldClicked2();
};

#endif // ITEMWIDGET_H
