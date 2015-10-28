#include "itemwidget.h"

ItemWidget::ItemWidget(QString id, QString name, QString picName, MESSAGE msg, QString content,
                       QString url, QWidget *parent)
    : QWidget(parent) ,_id(id), _url(url)
{
    _isRead = false;
    initUi();
    initLayout();
    initConnect();
    setName(name);
    setPic(picName);
    setMsgStyle(msg);
    setContent(content);

}

ItemWidget::ItemWidget(QString id, QString name, QString picName, MESSAGE msg, QString content,
                       QString url, QString day,QString time, QWidget *parent)
    : QWidget(parent) ,_id(id), _url(url)
{
    _isRead = false;
    initUi();
    initLayout();
    initConnect();
    setName(name);
    setPic(picName);
    setMsgStyle(msg);
    setContent(content);
    setDayAndtime(day, time);
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setName(QString name)
{
    _nameBtn->setText(name);
}

void ItemWidget::setPic(QString picName)
{
    _pic.load(picName);

    _picLabel->setPixmap(_pic.scaled(_picLabel->width(),_picLabel->height()));
}

void ItemWidget::setMsgStyle(MESSAGE msg)
{
    QString msgString;
    switch(msg)
    {
        case GOOD_PIC:  msgString = tr("赞了你的图片");        break;
        case GOOD_BLOG: msgString = tr("赞了你的博客");        break;
        case SPEAK_PIC: msgString = tr("评论了你的图片");       break;
        case SPEAK_BLOG:    msgString = tr("评论了你的博客");   break;
        case PRIVATE_SPEAK: msgString = tr("私信了你");        break;
        case AT_YOU : msgString = tr("@了你");        break;
        default: msgString = tr("给了你消息");                    break;
    }

    _msgStyleBtn->setText(msgString);
}

void ItemWidget::setContent(QString content)
{
    _contentBtn->setText(content);
}

void ItemWidget::setDayAndtime(QString day, QString time)
{
    _day->setText(day);
    _time->setText(time);
}

void ItemWidget::setIsRead(bool read)
{
    _isRead = read;
    _readMark->setVisible(_isRead);
}

bool ItemWidget::getIsRead()
{
    return _isRead;
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

    _contentBtn = new QPushButton(this);
    _contentBtn->setObjectName("contentBtn");
    _contentBtn->setCursor(Qt::PointingHandCursor);

    QDateTime time = QDateTime::currentDateTime();
    _day = new QLabel(this);
    _day->setText(time.toString("yyyy-MM-dd"));

    _time = new QLabel(this);
    _time->setText(time.toString("hh:mm:ss"));

    _readMark = new QLabel(this);
    _readMark->setObjectName("readMark");
    _readMark->setText("标记已读");
    _readMark->setVisible(false);
}

void ItemWidget::initLayout()
{
    _mainLayout = new QHBoxLayout(this);

    _mainLayout->addWidget(_picLabel);
    _mainLayout->addWidget(_nameBtn);
    _mainLayout->addSpacing(2);
    _mainLayout->addWidget(_msgStyleBtn);
    _mainLayout->addSpacing(2);
    _mainLayout->addWidget(_contentBtn);
    _mainLayout->addStretch();
    _mainLayout->addWidget(_readMark);
    _mainLayout->addWidget(_day);
    _mainLayout->addWidget(_time);

    this->setLayout(_mainLayout);
    this->setObjectName("itemWidget");
}

void ItemWidget::initConnect()
{
    connect(_picLabel, SIGNAL(clicked()), this, SLOT(handldClicked1()));
    connect(_nameBtn, SIGNAL(clicked()), this, SLOT(handldClicked1()));
    connect(_contentBtn, SIGNAL(clicked()), this, SLOT(handldClicked2()));
    connect(_msgStyleBtn, SIGNAL(clicked()), this, SLOT(handldClicked2()));
}

void ItemWidget::handldClicked1()
{
    emit clickBlogOwner(_id);
}

void ItemWidget::handldClicked2()
{
    emit clickUrl(_url);
}
