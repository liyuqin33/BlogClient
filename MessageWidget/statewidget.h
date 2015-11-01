#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QStackedWidget>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include "itemwidget.h"
#include "message.h"

#define PAGE_COUNT 11

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    StateWidget(QWidget *parent = 0);
    StateWidget(int allCount, int noReadCount, QWidget *parent = 0);
    ~StateWidget();
    //id, 博主名，图片路径名， 消息类型， 消息内容， 消息url， 是否已读
    void addNews(MESSAGE msg);

private:
    void initUi();
    void initLayout();
    void initConnect();

    QPushButton *_noReadBtn;                //未读通知
    QPushButton *_clearBtn;                 //清空所有的通知

    QWidget *_sWidget;                      //动态更新的widget
    QVBoxLayout *_sLayout;                  //动态更新的widget的layout
    QPushButton *_previousBtn;              //上一页按钮
    QPushButton *_nextBtn;                  //下一页按钮
    QList<ItemWidget*> _itemWidgetList;     //存放所有的消息
    QWidget *_spacer;                   //弹簧
    int _height;                            //itemwidget的高度

    int _allCount;                          //通知总数
    int _noReadCount;                       //未读通知
    int _nowPage;                           //当前页

signals:
    void clickedUrl(QString);
    void clickedBlogOwner(QString);

public slots:
    void clickedNextBtn();
    void clickedPreviousBtn();
    void updatePage(int page);
    void clickedNoReadBtn();
    void clickedClearBtn();
    void updateNoReadCount();

};

#endif // STATEWIDGET_H
