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

   for(int i = 0 ; i < 30; i++)
   {
       if(i%5 == 0)
       {
           _firstWidget->addNews(QString("000%1").arg(i), "蝶舞Dre", ":/Image/head1.jpg",
                                 SPEAK_BLOG, "这篇博客写的真好", "");
           _secondWidget->addNews(QString("000%1").arg(i), "蝶舞Dre", ":/Image/head1.jpg",
                                 PRIVATE_SPEAK, "这篇博客写的真好", "");
       }
       else
       {
           _firstWidget->addNews(QString("000%1").arg(i), "蝶舞Dre", ":/Image/head2.jpg",
                                 AT_YOU, "哈哈，祝你天天开心", "");
           _secondWidget->addNews(QString("000%1").arg(i), "蝶舞Dre", ":/Image/head3.jpg",
                                 PRIVATE_SPEAK, "哈哈，祝你天天开心", "");
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
