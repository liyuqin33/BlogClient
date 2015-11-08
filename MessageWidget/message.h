#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

enum MESSAGE_TYPE
{
    GOOD_PIC,
    GOOD_BLOG,
    SPEAK_PIC,
    SPEAK_BLOG,
    PRIVATE_SPEAK,
    AT_YOU
};  //赞图片，赞博客，评论图片，评论博客，私信

struct MESSAGE
{
    QString id;
    QString name;
    QString picHeadName;
    QString content;
    QString picName;
    QString url;
    QString day;
    QString time;
    bool isRead;
    MESSAGE_TYPE type;
};



#endif // MESSAGE_H
