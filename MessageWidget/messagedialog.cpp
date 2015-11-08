#include "messagedialog.h"

MessageDialog::MessageDialog(MESSAGE &msg, QWidget *parent)
    : QDialog(parent)
    , _msg(msg)
{
    _isLeftMouseBtnPressed = false;
    init();
    initData();
    _msg.isRead = true;
}

MessageDialog::~MessageDialog()
{

}

void MessageDialog::init()
{
    this->setFixedWidth(350);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 5);

    /***********上板块布局*************/
    up_frame = new QWidget(this);
    up_frame->setObjectName(QStringLiteral("up_frame"));

    up_horizontalLayout = new QHBoxLayout(up_frame);
    up_horizontalLayout->setContentsMargins(0, 0, 0, 0);
    up_horizontalLayout->setSpacing(0);
    up_horizontalLayout->setObjectName(QStringLiteral("up_horizontalLayout"));

    horizontalSpacer = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    up_horizontalLayout->addItem(horizontalSpacer);

    closeBtn = new QPushButton(this);
    closeBtn->setObjectName(QStringLiteral("closeBtn"));
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    closeBtn->setIcon(QIcon(QString(":/Image/close.png")));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    up_horizontalLayout->addWidget(closeBtn);

    gridLayout->addWidget(up_frame, 0, 0, 1, 2);

    /***********左板块布局**************/
    left_verticalLayout = new QVBoxLayout();
    left_verticalLayout->setContentsMargins(5, 0, 0, 0);
    left_verticalLayout->setObjectName(QStringLiteral("left_verticalLayout"));

    headLab = new PicLabel(this);
    connect(headLab, SIGNAL(clicked()), this, SLOT(clickedMoreBtn()));
    headLab->setObjectName(QStringLiteral("headLab"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(headLab->sizePolicy().hasHeightForWidth());
    headLab->setSizePolicy(sizePolicy);
    headLab->setMinimumSize(QSize(60, 60));
    headLab->setMaximumSize(QSize(60, 60));
    headLab->setPixmap(QPixmap(QString::fromUtf8(":/Image/head1.jpg")));

    left_verticalLayout->addWidget(headLab);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    left_verticalLayout->addItem(verticalSpacer);

    gridLayout->addLayout(left_verticalLayout, 1, 0, 1, 1);

    /***********右板块布局**************/
    right_verticalLayout = new QVBoxLayout();
    right_verticalLayout->setObjectName(QStringLiteral("right_verticalLayout"));
    right_verticalLayout->setContentsMargins(0, 0, 5, 0);
    horizontalLayout_1 = new QHBoxLayout();
    horizontalLayout_1->setSpacing(0);
    horizontalLayout_1->setObjectName(QStringLiteral("horizontalLayout_1"));

    nameLab = new PicLabel(this);
    connect(nameLab, SIGNAL(clicked()), this, SLOT(clickedMoreBtn()));
    nameLab->setObjectName(QStringLiteral("nameLab"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(nameLab->sizePolicy().hasHeightForWidth());
    nameLab->setSizePolicy(sizePolicy1);
    nameLab->setMaximumSize(QSize(16777215, 15));

    horizontalLayout_1->addWidget(nameLab);

    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_1->addItem(horizontalSpacer_4);


    right_verticalLayout->addLayout(horizontalLayout_1);

    contentLab = new PicLabel(this);
    connect(contentLab, SIGNAL(clicked()), this, SLOT(clickedMoreBtn()));
    contentLab->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    contentLab->setWordWrap(true);
    contentLab->setObjectName(QStringLiteral("contentLab"));

    right_verticalLayout->addWidget(contentLab);

    picWidget = new QFrame(this);
    picWidget->setObjectName(QStringLiteral("picWidget"));
    picWidget->setFrameShape(QFrame::StyledPanel);
    picWidget->setFrameShadow(QFrame::Raised);
    picWidget->hide();
    picWidget->setFixedHeight(80);

    right_verticalLayout->addWidget(picWidget);

    gridLayout->addLayout(right_verticalLayout, 1, 1, 1, 1);

}

void MessageDialog::initData()
{
    headLab->setPixmap(QPixmap(_msg.picHeadName).scaled(headLab->width(), headLab->height()));
    nameLab->setText(_msg.name);
    contentLab->setText(_msg.content);
    if(_msg.picName != "")
    {
        picWidget->show();
        QLabel *pic = new QLabel;
        QHBoxLayout *hLayout = new QHBoxLayout(picWidget);
        hLayout->setContentsMargins(0, 0, 0, 0);
        hLayout->addWidget(pic);
        pic->setPixmap(QPixmap(_msg.picName).scaled(picWidget->height(), picWidget->height()));

    }
}

void MessageDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() < up_frame->height() && event->x() < up_frame->width())
    {
        if(_isLeftMouseBtnPressed)
        {
            this->move(this->pos() + (event->globalPos() - _ptLast));
            _ptLast = event->globalPos();
            event->accept();
        }
    }
    event->ignore();
}

void MessageDialog::mousePressEvent(QMouseEvent *event)
{
    _isLeftMouseBtnPressed = true;
    _ptLast = event->globalPos();
    event->ignore();
}

void MessageDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(_isLeftMouseBtnPressed)
        _isLeftMouseBtnPressed = false;
    event->ignore();
}

void MessageDialog::paintEvent(QPaintEvent *)
{
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 5, 5);
    setMask(bmp);
}

void MessageDialog::clickedMoreBtn()
{
    QDesktopServices::openUrl(QUrl("www.baidu.com"));
}

