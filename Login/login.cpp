#include "login.h"
//接口数据类
#include "MyUtility.h"
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
//布局事件类
#include "nofocusframedelegate.h"
#include "accountitem.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>
//位置时间动画类
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QMovie>
//调试类
#include <QDebug>

Login::Login(QWidget *parent,bool isFlash):
    QDialog(parent),
    ui(new Ui::Login),
    _isFlash(isFlash)
{
    initUI();//加载UI
    initData();//初始化数据
    initTray();//加载系统托盘
    loadUser();//加载本地用户
    initLink();//加载信号
}
Login::~Login()
{
    delete ui;
}

//加载函数
void Login::initData()//初始化数据
{
    _isLeftMouseBtnPressed = false;
    _isOldAutoLogin = false;
}
void Login::initUI()//加载UI
{
    //设置窗口
    setFixedSize(300,350);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);
    show();
    //渐变动画
    if(_isFlash)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start();
    }
    //ui
    ui->setupUi(this);
    ui->loginWidget->show();
    ui->userNameBox->lineEdit()->setPlaceholderText("Input Your User");//提示
    //qss
    /*
     * 字体、布局、Tab、鼠标变化在设计窗口设置，其他样式在qss设置
    */
}
void Login::initTray()//加载系统托盘
{
    _tray=new Tray((QWidget*)this->parent());
	connect(_tray, &Tray::showWindow, this, &Login::clickShowBtn);
	connect(_tray, &Tray::closeWindow, this, &Login::clickCloseBtn);
    connect(this, &Login::accepted, _tray, &Tray::logned);
}

void Login::initLink()//加载信号
{
    ui->loginBtn->setShortcut(QKeySequence::InsertParagraphSeparator);  //快捷键
    ui->userNameBox->installEventFilter(this);  //安装过滤器
    connect(ui->loginBtn, &QPushButton::toggled, this, &Login::login);//登陆
	//自动登陆——使用setClicked函数不会触发clicked信号
    connect(ui->autoLoginBox, &QCheckBox::clicked, this, &Login::premiseSavePassword);//自动登陆
    connect(ui->findPasswordBtn, &QPushButton::clicked, this, &Login::findPassword);//找回密码
    connect(ui->registerBtn, &QPushButton::clicked, this, &Login::registerUser);//注册
    connect(ui->closeBtn, &QPushButton::clicked, this, &Login::clickCloseBtn);//关闭
    connect(ui->minBtn, &QToolButton::clicked, this, &Login::clickMinBtn);//最小化
    connect(ui->userNameBox->lineEdit(), &QLineEdit::textEdited, this, &Login::changUser);//键盘修改用户名称
}

void Login::loadUser()//加载本地用户
{
    /*此处应把存于本地的用户载入框并判断是否登陆*/
    _listWidget = new QListWidget(this);
    _listWidget->setItemDelegate(new NoFocusFrameDelegate());
    ui->userNameBox->setEditable(true);
    ui->userNameBox->setModel(_listWidget->model());
    ui->userNameBox->setView(_listWidget);
    //读取文件
    QFile blogInfoFile(USER_INFO_JSON_PATH());
    qDebug()<<USER_INFO_JSON_PATH();
    blogInfoFile.open(QIODevice::ReadOnly);
    if (!blogInfoFile.isOpen())
    {
        _tray->showHit(tr("研发博客"), tr("欢迎使用研发博客"));
        qDebug() << "没有读取到用户信息";
        return ;
    }
    QByteArray  jsonData = blogInfoFile.readAll();
    blogInfoFile.close();
    //解析成json并载入用户条目
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        _tray->showHit(tr("研发博客"), tr("欢迎使用研发博客"));
        qDebug()<<"读取JSON格式文件错误！"<<error.error;
        return ;
    }
    QVariantMap result = doc.toVariant().toMap();
    foreach (QVariant user, result["UserInfo"].toList())
    {
		QVariantMap accountRecord = user.toMap();
        QListWidgetItem* item = new QListWidgetItem(_listWidget);
		AccountItem* accountWidget = new AccountItem(this, item,
				accountRecord["ID"].toString(),
				accountRecord["Password"].toString(),
                accountRecord["SavePassword"].toBool());
        connect(accountWidget, &AccountItem::showAccount, this, &Login::showAccount);
		connect(accountWidget, &AccountItem::removeAccount, this, &Login::removeAccount);
		_listWidget->setItemWidget(item, accountWidget);
        item->setSizeHint(accountWidget->size());//使item高度适应accountWidget的高度
    }
	if (_listWidget->count() > 0)
	{
		showAccount(_listWidget->item(0));
		if(result["AutoLogin"].toBool())
		{
            _isOldAutoLogin=true;    //载入时是否自动登录，决定离线登录和存储信息
			ui->autoLoginBox->setChecked(true);//根据该按钮状态保存信息，所以需要设置
            ui->loginBtn->animateClick();
		}
        else _isOldAutoLogin=false;
    }
}

//响应函数
void Login::clickShowBtn()//点击恢复响应函数
{
    setFixedSize(300,350);//为了适应系统不同
	this->raise();//顶置到最顶层，在始终最顶的下面
}

void Login::clickCloseBtn()//点击关闭响应函数
{
    saveUser(false);
    close();
}

void Login::clickMinBtn()//点击最小化响应函数
{
    setFixedSize(0,0);
}

void Login::login(bool isDown)//点击登陆响应函数(登陆或取消登陆)
{
    if(isDown)
    {
        if(_isOldAutoLogin)//自动登录
        {
            isSuccessfulLoaded(LOG_IN);
            return;
        }
        //发送信号给后台(判断是否加密)
        if(_listWidget->count()>0)
        {
            AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(ui->userNameBox->currentIndex()));
            if(ui->userNameBox->lineEdit()->text() == account->getID()
                    && ui->passwordEdit->text() == account->getPassword())
                emit logining(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), true);
            else
                emit logining(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), false);
        }
        else
        {
            emit logining(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), false);
        }
        qDebug()<<"登陆界面发出登陆信号"<<ui->userNameBox->lineEdit()->text()<<ui->passwordEdit->text();

        //登陆中界面
        showLoginInterface();
        //该句调试用-------------------------------------
        //调试的地方换在timesUp()
    }
    else
    {
        emit unLogining();
        endLoginInterface();
    }
}
void Login::registerUser()//点击注册响应函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
}
void Login::findPassword()//点击找回密码响应函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
}

void Login::premiseSavePassword(bool isPitchOn)//自动登陆必须记住密码（不会发出信号）
{
	if(isPitchOn) ui->savePasswordBox->setChecked(isPitchOn);
}

void Login::changUser(const QString & editUser)//键盘修改用户名称
{
    qDebug()<<editUser<<_listWidget->count();

    for(int i=0; i<_listWidget->count(); i++)//相同变选择
    {
        AccountItem* findItem=((AccountItem*)_listWidget->itemWidget(_listWidget->item(i)));
        if(editUser == findItem->getID())
        {
            showAccount(_listWidget->item(i));
            return;
        }
    }
    ui->passwordEdit->clear();
    ui->autoLoginBox->setChecked(false);
    ui->savePasswordBox->setChecked(false);
}

//用户交互-QComboBox
void Login::showAccount(QListWidgetItem *item)//将选项文本显示在QComboBox当中
{
    AccountItem *account = (AccountItem*)_listWidget->itemWidget(item);
    ui->userNameBox->hidePopup();
	ui->userNameBox->setCurrentIndex(_listWidget->row(item));
	ui->userNameBox->setEditText(account->getID());
	ui->passwordEdit->setText(account->getPassword());//详见saveAccount的注释
    ui->savePasswordBox->setChecked(account->getIsSavePassword());
}
void Login::removeAccount(QListWidgetItem *item)//删除帐号时，弹出提示框，与用户进行交互，告知是否确定要删除此帐号的所有信息！
{
    ui->userNameBox->hidePopup();
	QMessageBox message(QMessageBox::Question, "询问", "是否删除？",
						QMessageBox::Yes | QMessageBox::No, NULL);
	if(message.exec() == QMessageBox::Yes)
    {
		bool updateItem = false;
		if (ui->userNameBox->currentText() ==
				((AccountItem*)_listWidget->itemWidget(item))->getID())//当删除当前项时
		{
			updateItem = true;//位置变动
		}
		delete item;
        if (updateItem)//把第一项顶置
		{
			AccountItem *firstAccountWidget = (AccountItem*)_listWidget->itemWidget(_listWidget->item(0));
			ui->userNameBox->setEditText(firstAccountWidget->getID());
			ui->passwordEdit->setText(firstAccountWidget->getPassword());
			ui->savePasswordBox->setChecked(firstAccountWidget->getIsSavePassword());
		}
	}
}

//用户交互-登录中
void Login::showLoginInterface()
{
    //重新布局
    ui->userNameBox->hide();
    ui->passwordEdit->hide();
    ui->registerBtn->hide();
    ui->findPasswordBtn->hide();
    ui->savePasswordBox->hide();
    ui->autoLoginBox->hide();
    ui->hintLabel->setText("");
    //文字
    ui->hintLabel->setText("");
    ui->loginBtn->setText("取消登录");
    ui->loginBtn->setShortcut(QKeySequence::InsertParagraphSeparator);  //更改文本快捷键消失，重设
    //头像窗口
    _headWidget = new QWidget(ui->loginWidget);
    int width = 120;
    int height = 120;
    int x = this->width() / 2 - width / 2;
    int y = this->height() / 2 - height / 2 - 10;
    //转动边框(120*120)
    _headLineMovie = new QMovie(":/Image/login/head_line.gif");
    _headLine= new QLabel(_headWidget);
    _headLine->setMovie(_headLineMovie);
    _headLine->setGeometry(0,0,120,120);
    _headLine->show();
    //头像图片（100*100）
    _headPix= new QLabel(_headWidget);
    _headPix->setStyleSheet ("border:1px groove gray;"
                             "border-radius:50px;"
                             "background-repeat:no-repeat;"
                             "background-image: url(:/Image/login/head_nomal.jpg);");
    _headPix->setGeometry(10,10,100,100);
    _headPix->show();
    if(_isFlash)//动画
    {
        QPropertyAnimation *animation = new QPropertyAnimation(_headWidget, "geometry");
        animation->setDuration(500);
        animation->setStartValue(QRect(-width, y, width, height));
        animation->setKeyValueAt(0.15, QRect(0, y, width, height));
        animation->setKeyValueAt(0.5, QRect(x+5, y, width, height));
        animation->setKeyValueAt(0.75, QRect(x-5, y, width, height));
        animation->setEndValue(QRect(x, y, width, height));
        animation->start();
    }
    else//无动画
    {
        _headWidget->setFixedSize(120, 120);
        _headWidget->move(this->width() / 2 - _headWidget->width() / 2,
                       this->height() / 2 - _headWidget->height() / 2 - 10);
    }
    _headWidget->show();
    _headLineMovie->start();
    //时间循环(10秒200帧)
    _timeLine = new QTimeLine(10000, this);
    _timeLine->setFrameRange(0, 200);
    _timeLine->setCurveShape(QTimeLine::LinearCurve);
    connect(_timeLine, &QTimeLine::frameChanged, this, &Login::timesUp);
    _timeLine->start();
}


void Login::endLoginInterface()
{
    qDebug()<<"取消登陆";
    //重新布局
    disconnect(_timeLine, &QTimeLine::frameChanged, this, &Login::timesUp);
    ui->userNameBox->show();
    ui->passwordEdit->show();
    ui->registerBtn->show();
    ui->findPasswordBtn->show();
    ui->savePasswordBox->show();
    ui->autoLoginBox->show();
    delete _headWidget;
    ui->hintLabel->setText("");
    ui->loginBtn->setText("登录");
    ui->loginBtn->setShortcut(QKeySequence::InsertParagraphSeparator);  //更改文本快捷键消失，重设
}

void Login::timesUp(int time)
{
    if(time%2==0) qDebug()<<"第"<<time/20.0<<"秒";
    //文字
    if(time >= 10 && time%10==0)//0.5秒一次
    {
        if(ui->hintLabel->text()=="")
            ui->hintLabel->setText("登录中");
        else if(ui->hintLabel->text()=="登录中...")
            ui->hintLabel->setText("登录中");
        else
            ui->hintLabel->setText(ui->hintLabel->text()+".");
    }
    if(time>=200)//时间到，登陆超时
    {
        endLoginInterface();
        isSuccessfulLoaded(ERROR_UNNET);
    }
    //该句调试用！！！！！
    if(time >= 60)//三秒
    {
        isSuccessfulLoaded(LOG_IN);
    }
}

//后台交互
void Login::isSuccessfulLoaded(Load type)//后台返回是否能登陆对接的槽函数
{
    disconnect(ui->loginBtn, &QPushButton::toggled, this, &Login::login);//断开连接以便按钮弹起
    switch(type)
    {
        case LOG_IN:
        {
            //移除托盘连接，储存用户登录信息
            disconnect(_tray, &Tray::showWindow, this, &Login::clickShowBtn);
            disconnect(_tray, &Tray::closeWindow, this, &Login::clickCloseBtn);
            if(_isOldAutoLogin)//自动登陆
            {
                saveUser(true);
                this->Login::done();
                break;
            }
            disconnect(_timeLine, &QTimeLine::frameChanged, this, &Login::timesUp);
            _tray->showHit(tr("研发博客-登录"), tr("登录成功"));
            addCurrentUser();
            saveUser(true);
            //关闭渐变
            if(_isFlash)
            {
                //动画
                QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
                animation->setDuration(1000);
                animation->setStartValue(1);
                animation->setEndValue(0);
                animation->start();
                connect(animation, &QPropertyAnimation::finished, this, Login::done);
            }
            else
                this->Login::done();
            return;
        }
        case ERROR_PASSWORD:
            _tray->showHit(tr("研发博客-登录"), tr("密码错误"));
            ui->hintLabel->setText("密码错误");
            ui->passwordEdit->clear();
            ui->passwordEdit->setFocus();
            ui->loginBtn->setChecked(false);
            break;
        case ERROR_UNNET:
            _tray->showHit(tr("研发博客-登录"), tr("网络超时"));
            ui->hintLabel->setText("网络超时");
            ui->loginBtn->setChecked(false);
            break;
        case ERROR_USER:
            _tray->showHit(tr("研发博客-登录"), tr("用户名不存在"));
            ui->hintLabel->setText("用户名不存在");
            ui->passwordEdit->clear();
            ui->userNameBox->setFocus();
            ui->loginBtn->setChecked(false);
            break;
    }
    connect(ui->loginBtn, &QPushButton::toggled, this, &Login::login);
}

//结束
void Login::done()//结束时的槽函数（登陆并关闭）
{
    QDialog::done(QDialog::Accepted);
}

void Login::addCurrentUser()
{
	QString user = ui->userNameBox->lineEdit()->text();
	QListWidgetItem *item = nullptr;
	for (int i = 0; i != _listWidget->count(); ++i)
	{
		item = _listWidget->item(i);
		if(((AccountItem*)_listWidget->itemWidget(item))->getID() == user)//如果相同
		{
			delete item;//删除
			break;//ASSERT：原listWidget不重复
		}
	}
	item = new QListWidgetItem();
	AccountItem *account = new AccountItem(_listWidget, item, user,
										   ui->passwordEdit->text(),
										   ui->savePasswordBox->isChecked());
	_listWidget->insertItem(0, item);
	_listWidget->setItemWidget(item, account);
}

void Login::saveUser(bool isSuccessfulLoaded)//添加本地用户
{
    /*此处应把成功登陆用户存于本地*/
    QVariantList userList;
    //生成其他json
    for(int i=0; i<_listWidget->count(); i++)
    {
        QVariantMap jsonItem;
        AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(i));
		saveAccount(jsonItem, account->getID(), account->getPassword(), account->getIsSavePassword());
		userList << jsonItem;
    }
    QVariantMap all;
    all.insert("UserInfo", userList);
    if(isSuccessfulLoaded)  //登录成功记录用户是否自动登陆
        all.insert("AutoLogin", ui->autoLoginBox->isChecked());
    else    //失败保留原来的数据
        all.insert("AutoLogin", _isOldAutoLogin);

    QJsonDocument doc = QJsonDocument::fromVariant(all);
    if (!doc.isNull())
    {
        qDebug() << doc.toJson();
    }
    //写入文件
    QFile blogInfoFile(USER_INFO_JSON_PATH());
    blogInfoFile.open(QIODevice::WriteOnly);
    if (!blogInfoFile.isOpen())
    {
        QMessageBox::critical(this, "保存文件错误", "保存用户帐号信息失败！");
        qDebug() << "写入时没有打开到用户文件";
        return ;
    }
    blogInfoFile.write(doc.toJson());
	blogInfoFile.close();
}

void Login::saveAccount(QVariantMap &item,
                        const QString &id,
                        const QString &password,
                        bool isSavePassword)
{
    item.insert("ID", id);
    item.insert("SavePassword", isSavePassword);
    if (isSavePassword)
		item.insert("Password", password);
}



//鼠标拖动事件
void Login::mouseReleaseEvent(QMouseEvent *event)
{
    if(_isLeftMouseBtnPressed)
        _isLeftMouseBtnPressed = false;
    event->ignore();
}
void Login::mousePressEvent(QMouseEvent *event)
{
    _isLeftMouseBtnPressed = true;
    _ptLast = event->globalPos();
    event->ignore();
}
void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(_isLeftMouseBtnPressed)
    {
        QPoint ptNew = event->globalPos();
        ptNew -= _ptLast;
        ptNew += pos();
        move(ptNew);
        _ptLast  = event->globalPos();
    }
    event->ignore();
}


//事件过滤器使滚轮失效
bool Login::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->userNameBox)
    {
        if (event->type() == QEvent::Wheel)
           return true;
        else
           return false;
    }
    else
        return Login::eventFilter(obj, event);
}

