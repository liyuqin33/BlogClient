#include "statewidget.h"

StateWidget::StateWidget(QWidget *parent)
    : QWidget(parent)
    , _allCount(0)
    , _nowPage(1)
{
    _noReadCount = 0;
    initUi();
    initLayout();
    initConnect();
}

StateWidget::StateWidget(int allCount, int noReadCount, QWidget *parent)
    : QWidget(parent)
    , _allCount(allCount)
    , _noReadCount(noReadCount)
    , _nowPage(1)
{
    initUi();
    initLayout();
    initConnect();
}

StateWidget::~StateWidget()
{

}

void StateWidget::addNews(QString id, QString name, QString picName, MESSAGE msg,
                          QString content, QString url, bool isRead)
{
    _allCount++;

    if(!isRead)
        _noReadCount++;
    //一则消息
    ItemWidget *itemWidget = new ItemWidget(id, name, picName, msg, content, url);
    itemWidget->hide();
    itemWidget->setIsRead(isRead);
    //转发信号
    connect(itemWidget, SIGNAL(clickBlogOwner(QString)), this, SIGNAL(clickedBlogOwner(QString)));
    connect(itemWidget, SIGNAL(clickUrl(QString)), this, SIGNAL(clickedUrl(QString)));
    _itemWidgetList.insert(0, itemWidget);

    updatePage(1);
    updateNoReadCount();
}

void StateWidget::addNews(QString id, QString name, QString picName, MESSAGE msg,
                          QString content, QString url, QString day, QString time, bool isRead)
{
    _allCount++;

    if(!isRead)
        _noReadCount++;
    //一则消息
    ItemWidget *itemWidget = new ItemWidget(id, name, picName, msg, content, url, day, time);
    itemWidget->hide();
    itemWidget->setIsRead(isRead);
    //转发信号
    connect(itemWidget, SIGNAL(clickBlogOwner(QString)), this, SIGNAL(clickedBlogOwner(QString)));
    connect(itemWidget, SIGNAL(clickUrl(QString)), this, SIGNAL(clickedUrl(QString)));
    _itemWidgetList.insert(0, itemWidget);

    updatePage(1);
    updateNoReadCount();
}

void StateWidget::initUi()
{
    _noReadBtn = new QPushButton(this);
    _noReadBtn->setObjectName(QStringLiteral("noReadBtn"));
    _noReadBtn->setText(QString("未读通知：%1").arg(_noReadCount));
    _noReadBtn->setCursor(Qt::PointingHandCursor);

    _markBtn = new QPushButton(this);
    _markBtn->setObjectName(QStringLiteral("markBtn"));
    _markBtn->setText("全部标记为已读");
    _markBtn->setCursor(Qt::PointingHandCursor);

    _clearBtn = new QPushButton(this);
    _clearBtn->setObjectName(QStringLiteral("clearBtn"));
    _clearBtn->setText("清空所有的通知");
    _clearBtn->setCursor(Qt::PointingHandCursor);

    _previousBtn = new QPushButton(this);
    _previousBtn->setObjectName(QStringLiteral("previousBtn"));
    _previousBtn->setText("上一页");
    _previousBtn->setCursor(Qt::PointingHandCursor);

    _nextBtn = new QPushButton(this);
    _nextBtn->setObjectName(QStringLiteral("nextBtn"));
    _nextBtn->setText("下一页");
    _nextBtn->setCursor(Qt::PointingHandCursor);

    _sWidget = new QWidget(this);
    _sLayout = new QVBoxLayout;
    _sWidget->setLayout(_sLayout);

    _spacer = new QWidget(this);
    _spacer->hide();

}

void StateWidget::initLayout()
{
    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addWidget(_noReadBtn);
    upLayout->addSpacing(10);
    upLayout->addWidget(_markBtn);
    upLayout->addSpacing(10);
    upLayout->addWidget(_clearBtn);
    upLayout->addStretch();

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addStretch();
    downLayout->addWidget(_previousBtn);
    downLayout->addWidget(_nextBtn);
    downLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(upLayout);
    mainLayout->addWidget(_sWidget);
    mainLayout->addLayout(downLayout);

    this->setLayout(mainLayout);
}

void StateWidget::initConnect()
{
    connect(_nextBtn, SIGNAL(clicked()), this, SLOT(clickedNextBtn()));
    connect(_previousBtn, SIGNAL(clicked()), this, SLOT(clickedPreviousBtn()));
    connect(_noReadBtn, SIGNAL(clicked()), this, SLOT(clickedNoReadBtn()));
    connect(_markBtn, SIGNAL(clicked()), this, SLOT(clickedMarkBtn()));
    connect(_clearBtn, SIGNAL(clicked()), this, SLOT(clickedClearBtn()));
}

void StateWidget::clickedNextBtn()
{
    if(( _allCount -  (_nowPage * PAGE_COUNT )) > 0 )
    {
        _nowPage++;
        updatePage(_nowPage);
    }
}

void StateWidget::clickedPreviousBtn()
{
    if(_nowPage > 1 )
    {
        _nowPage--;
        updatePage(_nowPage);
    }
}

void StateWidget::updatePage(int page)
{
    for(int i = 0;i < _itemWidgetList.count(); i++)
    {
        _sLayout->removeWidget(_itemWidgetList.at(i));
        _itemWidgetList.at(i)->hide();
    }

    _sLayout->removeWidget(_spacer);
    _spacer->hide();

    int i = (page - 1) * PAGE_COUNT;

    while(i < _itemWidgetList.count() && i < (page * PAGE_COUNT))
    {
        _sLayout->addWidget(_itemWidgetList.at(i));
        _itemWidgetList.at(i)->show();
        i++;
    }
    _height = _sWidget->height() / PAGE_COUNT;
    if(_sLayout->count() < PAGE_COUNT)
    {
        _spacer->setFixedHeight(_sWidget->height() - _sLayout->count() * _height);
        _spacer->show();
        _sLayout->insertWidget(_sLayout->count(), _spacer);
    }
}

void StateWidget::clickedNoReadBtn()
{

}

void StateWidget::clickedMarkBtn()
{
    if(!_itemWidgetList.isEmpty())
    {
        for(int i = 0; i < _itemWidgetList.count(); i++)
        {
            _itemWidgetList.at(i)->setIsRead(true);
        }
        _noReadCount = 0;
        updateNoReadCount();
    }
}

void StateWidget::clickedClearBtn()
{
    if(!_itemWidgetList.isEmpty())
    {
        for(int i = 0;i < _itemWidgetList.count(); i++)
        {
            _sLayout->removeWidget(_itemWidgetList.at(i));
            _itemWidgetList.at(i)->hide();
        }

        _sLayout->removeWidget(_spacer);
        _spacer->hide();
        _itemWidgetList.clear();
        _allCount = 0;
        _noReadCount = 0;
        updateNoReadCount();
    }
}

void StateWidget::updateNoReadCount()
{
    _noReadBtn->setText(QString("未读通知：%1").arg(_noReadCount));
}


