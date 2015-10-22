#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QFile>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "piclabel.h"

enum MESSAGE{GOOD_PIC, GOOD_BLOG, SPEAK_PIC, SPEAK_BLOG, PRIVATE_SPEAK, AT_YOU };
            //赞图片，赞博客，评论图片，评论博客，私信


class ItemWidget : public QWidget
{
    Q_OBJECT
public:
    ItemWidget(QString id, QString name, QString picName, MESSAGE msg, QString content,
               QString url, QWidget *parent = 0);
    ItemWidget(QString id, QString name, QString picName, MESSAGE msg, QString content, QString url,
               QString day, QString time, QWidget *parent = 0);
    ~ItemWidget();

    void setName(QString name);                     //设置名字
    void setPic(QString picName);                   //设置图片名
    void setMsgStyle(MESSAGE msg);                  //设置消息类型
    void setContent(QString content);               //设置评论内容
    void setDayAndtime(QString day, QString time);  //设置时间
    void setIsRead(bool read);                      //设置标记已读
    void setUrl(QString url);                       //设置url
    bool getIsRead();
private:
    void initUi();
    void initLayout();
    void initConnect();

    QString _id;                                    //id
    PicLabel *_picLabel;                            //图片label
    QPixmap _pic;                                   //图片pix
    QPushButton *_nameBtn;                          //名字按钮
    QPushButton *_msgStyleBtn;                      //消息类型按钮
    QPushButton *_contentBtn;                       //内容按钮
    QString _url;                                   //url
    QLabel *_day;                                   //日期
    QLabel *_time;                                  //时间
    QLabel *_readMark;                              //标记为已读
    bool _isRead;                                   //是否已读

    QHBoxLayout *_mainLayout;

signals:
    void clickBlogOwner(QString);
    void clickUrl(QString);

public slots:
    void handldClicked1();
    void handldClicked2();
};

#endif // ITEMWIDGET_H
