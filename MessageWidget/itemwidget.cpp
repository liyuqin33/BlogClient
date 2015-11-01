#include "itemwidget.h"

ItemWidget::ItemWidget(MESSAGE msg, QWidget *parent)
    : QWidget(parent)
    , _msg(msg)
{
    initUi();
    initLayout();
    initConnect();
    setName(msg.name);
    setPicHeadName(msg.picHeadName);
    setMsgStyle(msg.type);
    setDayAndtime(msg.day, msg.time);

}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setName(QString name)
{
    _nameBtn->setText(name);
}

void ItemWidget::setPicHeadName(QString picHeadName)
{
    if(picHeadName != "")
        _pic.load(picHeadName);
    else
        _pic.load(":/Image/head1.jpg");
    _picLabel->setPixmap(_pic.scaled(_picLabel->width(),_picLabel->height()));
}

void ItemWidget::setMsgStyle(MESSAGE_TYPE msg)
{
    QString msgString;
    switch(msg)
    {
        case GOOD_PIC:  msgString = tr("赞了你");        break;
        case GOOD_BLOG: msgString = tr("赞了你");        break;
        case SPEAK_PIC: msgString = tr("评论了你");       break;
        case SPEAK_BLOG:    msgString = tr("评论了你");   break;
        case PRIVATE_SPEAK: msgString = tr("私信了你");        break;
        case AT_YOU : msgString = tr("@了你");        break;
        default: msgString = tr("给了你消息");                    break;
    }

    _msgStyleBtn->setText(msgString);
}

void ItemWidget::setDayAndtime(QString day, QString time)
{
    if(day != "")
        _day->setText(day);
    if(time != "")
        _time->setText(time);
}

void ItemWidget::initUi()
{
    _pic.load(":/Image/head1.jpg");

    _picLabel = new PicLabel(this);
    _picLabel->setFixedSize(35,35);
    _picLabel->setPixmap(_pic.scaled(_picLabel->width(),_picLabel->height()));
    _picLabel->setCursor(Qt::PointingHandCursor);

    _nameBtn = new QPushButton(this);
    _nameBtn->setObjectName("nameBtn");
    _nameBtn->setCursor(Qt::PointingHandCursor);

    _msgStyleBtn = new QPushButton(this);
    _msgStyleBtn->setObjectName("msgBtn");
    _msgStyleBtn->setCursor(Qt::PointingHandCursor);

    QDateTime time = QDateTime::currentDateTime();
    _day = new QLabel(this);
    _day->setText(time.toString("yy/MM/dd"));
    _day->setVisible(false);

    _time = new QLabel(this);
    _time->setText(time.toString("hh:mm:ss"));


}

void ItemWidget::initLayout()
{
    _mainLayout = new QHBoxLayout(this);

    _mainLayout->addWidget(_picLabel);
    _mainLayout->addWidget(_nameBtn);
    _mainLayout->addSpacing(2);
    _mainLayout->addWidget(_msgStyleBtn);
    _mainLayout->addStretch();
    _mainLayout->addWidget(_day);
    _mainLayout->addWidget(_time);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);
    this->setObjectName("itemWidget");
}

void ItemWidget::initConnect()
{
    connect(_picLabel, SIGNAL(clicked()), this, SLOT(handldClicked1()));
    connect(_nameBtn, SIGNAL(clicked()), this, SLOT(handldClicked1()));
    connect(_msgStyleBtn, SIGNAL(clicked()), this, SLOT(handldClicked2()));
}

void ItemWidget::handldClicked1()
{
    emit clickBlogOwner(_msg.id);
    qDebug () << _msg.id;
}

void ItemWidget::handldClicked2()
{
    MessageDialog *dig = new MessageDialog(_msg);
    dig->exec();
}
