#include "widget.h"

MessageWidget::MessageWidget(QWidget *parent) :
    QWidget(parent)
  , _tabWidget(new QTabWidget(this))
  , _firstWidget(new StateWidget)
  , _secondWidget(new StateWidget)
{
    initUi();
    initLayout();
}

MessageWidget::~MessageWidget()
{

}

void MessageWidget::initUi()
{
    _tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    _tabWidget->setObjectName("tabWidget");

   QString content("你写的真不错，啊啊啊，呵呵呵，哒哒哒，嚒嚒嚒，哒哒哒，咯咯咯，葛葛葛，还需要改一些方面");
   QString day(QDateTime::currentDateTime().toString("yy/MM/dd"));
   QString time(QDateTime::currentDateTime().toString("hh:mm:ss"));
   for(int i = 0 ; i < 30; i++)
   {
       if(i%5 == 0)
       {
           MESSAGE msg = {QString("000%1").arg(i), "蝶舞Dre", ":/Image/head1.jpg", content,
                          "", "www.baidu.com", day, time, false, GOOD_BLOG};

           _firstWidget->addNews(msg);
           _secondWidget->addNews(msg);
       }
       else
       {
           MESSAGE msg = {QString("000%1").arg(i), "蝶舞Dre", ":/Image/head2.jpg", content,
                          ":/Image/bg.jpg", "www.baidu.com", day, time, false, GOOD_PIC};
           _firstWidget->addNews(msg);
           _secondWidget->addNews(msg);
       }
   }


   _tabWidget->addTab(_firstWidget, "社交动态");
   _tabWidget->addTab(_secondWidget, "系统通知");

}

void MessageWidget::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_tabWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);
}
