#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    _isLock = false;
    _isMax = false;
    initUI();
    initLayout();
    initConnect();
}

TitleBar::~TitleBar()
{

}

void TitleBar::setName(QString s)
{
    _nameBtn->setText(s);
}

QString TitleBar::getName()
{
    return _nameBtn->text();
}

bool TitleBar::getLockFlag()
{
    return _isLock;
}

void TitleBar::initUI()
{
    _nameBtn = new QLabel(this);
    _nameBtn->setText(tr("蝶舞Dre"));
    _nameBtn->setFixedHeight(height());

    _maxIcon = QIcon(QString(":/BlogImages/fullscreen.png"));
    _normalIcon = QIcon(QString(":/BlogImages/app.png"));;
    _unlockIcon = QIcon(QString(":/BlogImages/unlock.png"));
    _lockIcon = QIcon(QString(":/BlogImages/lock.png"));

    _logoButton = new QToolButton();
    _logoButton->setObjectName("logoButton");
    _logoButton->setFocusPolicy(Qt::NoFocus);
    _logoButton->setIcon(QIcon(QString(":/BlogImages/logo.png")));
    _logoButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _settingButton = new QToolButton(this);
    _settingButton->setObjectName("settingButton");
    _settingButton->setFocusPolicy(Qt::NoFocus);
    _settingButton->setIcon(QIcon(QString(":/BlogImages/down.png")));
    _settingButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _skinButton = new QToolButton(this);
    _skinButton->setObjectName("skinButton");
    _skinButton->setFocusPolicy(Qt::NoFocus);
    _skinButton->setIcon(QIcon(QString(":/BlogImages/shirt.png")));
    _skinButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _lockButton = new QToolButton(this);
    _lockButton->setObjectName("lockButton");
    _lockButton->setFocusPolicy(Qt::NoFocus);
    _lockButton->setIcon(QIcon(QString(":/BlogImages/unlock.png")));
    _lockButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));


    _minButton = new QToolButton(this);
    _minButton->setObjectName("minButton");
    _minButton->setFocusPolicy(Qt::NoFocus);
    _minButton->setIcon(QIcon(QString(":/BlogImages/min.png")));
    _minButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _maxButton = new QToolButton(this);
    _maxButton->setObjectName("maxButton");
    _maxButton->setFocusPolicy(Qt::NoFocus);
    _maxButton->setIcon(QIcon(QString(":/BlogImages/app.png")));
    _maxButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _closeButton = new QToolButton(this);
    _closeButton->setObjectName("closeButton");
    _closeButton->setFocusPolicy(Qt::NoFocus);
    _closeButton->setIcon(QIcon(QString(":/BlogImages/close.png")));
    _closeButton->setIconSize(QSize(TITLE_HEIGHT, TITLE_HEIGHT));

    _homeBtn = new QPushButton(this);
    _homeBtn->setFlat(true);
    _homeBtn->setCheckable(true);
    _homeBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _homeBtn->setText(tr("首页"));
    _homeBtn->setFocusPolicy(Qt::NoFocus);
    _homeBtn->setObjectName("checkable");

    _blogBtn = new QPushButton(this);
    _blogBtn->setFlat(true);
    _blogBtn->setCheckable(true);
    _blogBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _blogBtn->setText(tr("博客"));
    _blogBtn->setFocusPolicy(Qt::NoFocus);
    _blogBtn->setObjectName("checkable");

    _picBtn = new QPushButton(this);
    _picBtn->setFlat(true);
    _picBtn->setCheckable(true);
    _picBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _picBtn->setText(tr("照片墙"));
    _picBtn->setFocusPolicy(Qt::NoFocus);
    _picBtn->setObjectName("checkable");

    _personBtn = new QPushButton(this);
    _personBtn->setFlat(true);
    _personBtn->setCheckable(true);
    _personBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _personBtn->setText(tr("个人中心"));
    _personBtn->setFocusPolicy(Qt::NoFocus);
    _personBtn->setObjectName("checkable");

    _communitBtn = new QPushButton(this);
    _communitBtn->setFlat(true);
    _communitBtn->setCheckable(true);
    _communitBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _communitBtn->setText(tr("研发动态"));
    _communitBtn->setFocusPolicy(Qt::NoFocus);
    _communitBtn->setObjectName("checkable");

    _aboutBtn = new QPushButton(this);
    _aboutBtn->setFlat(true);
    _aboutBtn->setCheckable(true);
    _aboutBtn->setFixedHeight(TITLE_HEIGHT + 5);
    _aboutBtn->setText(tr("关于研发"));
    _aboutBtn->setFocusPolicy(Qt::NoFocus);
    _aboutBtn->setObjectName("checkable");

}

void TitleBar::initLayout()
{
    QGridLayout* mainLayout = new QGridLayout(this);
    QHBoxLayout* upLayout = new QHBoxLayout;
    upLayout->addStretch();
    upLayout->addWidget(_settingButton);
    upLayout->addWidget(_skinButton);
    upLayout->addWidget(_lockButton);
    upLayout->addWidget(_minButton);
    upLayout->addWidget(_maxButton);
    upLayout->addWidget(_closeButton);
    upLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout* downLayout = new QHBoxLayout;
    downLayout->addWidget(_nameBtn);
    downLayout->addWidget(_homeBtn);
    downLayout->addWidget(_blogBtn);
    downLayout->addWidget(_picBtn);
    downLayout->addWidget(_personBtn);
    downLayout->addWidget(_communitBtn);
    downLayout->addWidget(_aboutBtn);
    downLayout->addStretch();
    downLayout->setContentsMargins(0,0,0,0);


    mainLayout->addWidget(_logoButton,0,0);
    mainLayout->addLayout(downLayout,0,1);
    mainLayout->addLayout(upLayout,0,2);
    this->setLayout(mainLayout);

}

void TitleBar::initConnect()
{
    connect(_settingButton, SIGNAL(clicked()), this, SIGNAL(clickSettingBtn()));
    connect(_skinButton, SIGNAL(clicked()), this, SIGNAL(clickSkinBtn()));
    connect(_lockButton, SIGNAL(clicked()), this, SLOT(swithLockBtn()));
    connect(_maxButton, SIGNAL(clicked()), this, SLOT(swithMaxNormalBtn()));
    connect(_minButton, SIGNAL(clicked()), this, SIGNAL(clickMinBtn()));
    connect(_closeButton, SIGNAL(clicked()), this, SIGNAL(clickCloseBtn()));

    connect(_homeBtn, SIGNAL(clicked()), this, SLOT(clickHomeBtn()));
    connect(_blogBtn, SIGNAL(clicked()), this, SLOT(clickBlogBtn()));
    connect(_picBtn, SIGNAL(clicked()), this, SLOT(clickPicBtn()));
    connect(_personBtn, SIGNAL(clicked()), this, SLOT(clickPersonBtn()));
    connect(_communitBtn, SIGNAL(clicked()), this, SLOT(clickCommunitBtn()));
    connect(_aboutBtn, SIGNAL(clicked()), this, SLOT(clickAboutBtn()));
}

void TitleBar::swithMaxNormalBtn()
{
    if(_isMax)
    {
        _maxButton->setIcon(_normalIcon);
        emit clickNormalBtn();

    }
    else
    {
        _maxButton->setIcon(_maxIcon);
        emit clickMaxBtn();
    }

    _isMax = !_isMax;
}

void TitleBar::swithLockBtn()
{
    if(_isLock)
    {
        _lockButton->setIcon(_unlockIcon);
        emit clickUnlockBtn();
    }
    else
    {
        _lockButton->setIcon(_lockIcon);
        emit clickLockBtn();
    }

    _isLock = !_isLock;
}

void TitleBar::clickHomeBtn()
{
    _homeBtn->setChecked(true);
    _blogBtn->setChecked(false);
    _picBtn->setChecked(false);
    _personBtn->setChecked(false);
    _communitBtn->setChecked(false);
    _aboutBtn->setChecked(false);
    emit clickFunctionBtn(0);
}

void TitleBar::clickBlogBtn()
{
    _homeBtn->setChecked(false);
    _blogBtn->setChecked(true);
    _picBtn->setChecked(false);
    _personBtn->setChecked(false);
    _communitBtn->setChecked(false);
    _aboutBtn->setChecked(false);
    emit clickFunctionBtn(1);
}

void TitleBar::clickPicBtn()
{
    _homeBtn->setChecked(false);
    _blogBtn->setChecked(false);
    _picBtn->setChecked(true);
    _personBtn->setChecked(false);
    _communitBtn->setChecked(false);
    _aboutBtn->setChecked(false);
    emit clickFunctionBtn(2);
}

void TitleBar::clickPersonBtn()
{
    _homeBtn->setChecked(false);
    _blogBtn->setChecked(false);
    _picBtn->setChecked(false);
    _personBtn->setChecked(true);
    _communitBtn->setChecked(false);
    _aboutBtn->setChecked(false);
    emit clickFunctionBtn(3);
}

void TitleBar::clickCommunitBtn()
{
    _homeBtn->setChecked(false);
    _blogBtn->setChecked(false);
    _picBtn->setChecked(false);
    _personBtn->setChecked(false);
    _communitBtn->setChecked(true);
    _aboutBtn->setChecked(false);
    emit clickFunctionBtn(4);
}

void TitleBar::clickAboutBtn()
{
    _homeBtn->setChecked(false);
    _blogBtn->setChecked(false);
    _picBtn->setChecked(false);
    _personBtn->setChecked(false);
    _communitBtn->setChecked(false);
    _aboutBtn->setChecked(true);
    emit clickFunctionBtn(5);
}








