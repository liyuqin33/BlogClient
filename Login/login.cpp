#include "login.h"
//接口数据类
#include "../Base/MyUtility.h"
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

#include <qt_windows.h>

Login::Login(QWidget *parent, bool isFlash)
	: QWidget(parent)
	, ui(new Ui::Login)
	, _isFlash(isFlash)
{
	this->setObjectName("loginWidget");
    initUI();//加载UI
    initData();//初始化数据
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
	this->setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(300,350);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);

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
    ui->userNameBox->lineEdit()->setPlaceholderText("Input Your User");//提示
	this->_titleHeight = ui->minBtn->y() + ui->minBtn->height();
    //qss
    /*
     * 字体、布局、Tab、鼠标变化在设计窗口设置，其他样式在qss设置
    */
}

void Login::initLink()//加载信号
{
    ui->loginBtn->setShortcut(QKeySequence::InsertParagraphSeparator);  //快捷键
    ui->userNameBox->installEventFilter(this);  //安装过滤器
	connect(ui->loginBtn, &QPushButton::toggled, this, &Login::userTryLogin);//登陆
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
		sendMessage("欢迎使用研发博客");
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
		sendMessage("欢迎使用研发博客");
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
                accountRecord["PixURL"].toString(),
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
	this->showNormal();
    this->activateWindow();
}

void Login::clickCloseBtn()//点击关闭响应函数
{
    saveUser(false);
    close();
}

void Login::clickMinBtn()//点击最小化响应函数
{
    this->showMinimized();
}

void Login::userTryLogin(bool isDown)//点击登陆响应函数(登陆或取消登陆)
{
    if(isDown)
    {
        if(_isOldAutoLogin)//自动登录
        {
            checkLoginMessage(LOG_IN,"","","");
            return;
        }
        //发送信号给后台(判断是否加密)
        if(_listWidget->count()>0)
        {
            AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(ui->userNameBox->currentIndex()));
            if(ui->userNameBox->lineEdit()->text() == account->getID()
                    && ui->passwordEdit->text() == account->getPassword())
				emit tryLogin(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), true);
            else
				emit tryLogin(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), false);
        }
        else
        {
			emit tryLogin(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), false);
        }
        qDebug()<<"登陆界面发出登陆信号"<<ui->userNameBox->lineEdit()->text()<<ui->passwordEdit->text();

        //登陆中界面
        showLoginInterface();
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

void Login::premiseSavePassword(bool isAutoLogin)//点击自动登陆单选框响应函数
{
	if(isAutoLogin) ui->savePasswordBox->setChecked(isAutoLogin);
}

void Login::changUser(const QString & editUser)//键盘修改用户名称响应函数
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
void Login::showLoginInterface()    //显示登陆界面
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
	_headWidget = new QWidget(this);
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
    if(_listWidget->count()>0)
    {
        AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(0));
        //用户有头像
        if(ui->userNameBox->lineEdit()->text() == account->getID() && (!account->getPixURL().isEmpty()))
            _headPix->setStyleSheet ("border:1px groove gray;border-radius:50px;background-repeat:no-repeat;"+
                                     QString("background-image: url(%1);").arg(account->getPixURL()));
        else    //默认头像
            _headPix->setStyleSheet ("border:1px groove gray;"
                                    "border-radius:50px;"
                                    "background-repeat:no-repeat;"
                                    "background-image: url(:/Image/login/head_nomal.jpg);");
    }
    else    //默认头像
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


void Login::endLoginInterface() //结束登陆界面
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

void Login::timesUp(int time)   //登陆时计时器
{
	qDebug() << time << endl;
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
        checkLoginMessage(ERROR_UNNET,"","",":/Image/head1.jpg");
    }
    //该句调试用！！！！！后面的字符串可以写入图片的地址进行调试使用
    if(time >= 60)//三秒
    {
        checkLoginMessage(LOG_IN, ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text(), "");
    }
}

//后台交互
void Login::checkLoginMessage(LoginMessage type, QString accountitem, QString password, QString headPix)//后台返回是否能登陆对接的槽函数
{
	disconnect(ui->loginBtn, &QPushButton::toggled, this, &Login::userTryLogin);//断开连接以便按钮弹起
    switch(type)
    {
        case LOG_IN:
        {
            if(_isOldAutoLogin)//自动登陆
            {
                saveUser(true);
				emit successfulLogin();
                break;
            }
            disconnect(_timeLine, &QTimeLine::frameChanged, this, &Login::timesUp);
			emit sendMessage("登录成功");
            addCurrentUser(accountitem, password, headPix);
            saveUser(true);
			emit successfulLogin();
            return;
        }
        case ERROR_PASSWORD:
			emit sendMessage("密码错误");
            ui->hintLabel->setText("密码错误");
            ui->passwordEdit->clear();
            ui->passwordEdit->setFocus();
            ui->loginBtn->setChecked(false);
            break;
        case ERROR_UNNET:
			emit sendMessage("网络超时");
            ui->hintLabel->setText("网络超时");
            ui->loginBtn->setChecked(false);
            break;
        case ERROR_USER:
			emit sendMessage("用户名不存在");
            ui->hintLabel->setText("用户名不存在");
            ui->passwordEdit->clear();
            ui->userNameBox->setFocus();
            ui->loginBtn->setChecked(false);
            break;
	}
}

void Login::addCurrentUser(QString user, QString password, QString pixURL)  //添加当前新用户
{
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
    AccountItem *account = new AccountItem(_listWidget, item, user, password, pixURL,
                                           ui->savePasswordBox->isChecked());
	_listWidget->insertItem(0, item);
	_listWidget->setItemWidget(item, account);
}

void Login::saveUser(bool isSuccessfulLoaded)//添加本地用户
{
    /*此处应把成功登陆用户存于本地*/
    QVariantList userList;
    for(int i=0; i<_listWidget->count(); i++)
    {
        QVariantMap jsonItem;
        AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(i));
        saveAccount(jsonItem, account->getID(), account->getPassword(), account->getPixURL(), account->getIsSavePassword());
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
        sendMessage("保存用户帐号信息失败");
        qDebug() << "写入时没有打开到用户文件";
        return ;
    }
    blogInfoFile.write(doc.toJson());
	blogInfoFile.close();
}

void Login::saveAccount(QVariantMap &item,    //写入时，插入一个用户
                        const QString &id,
                        const QString &password,
                        const QString &pixURL,
                        bool isSavePassword)
{
    item.insert("ID", id);
    item.insert("SavePassword", isSavePassword);
    if (!pixURL.isEmpty())
        item.insert("PixURL",pixURL);
    if (isSavePassword)
		item.insert("Password", password);
}

bool Login::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);//不使用的参数

	const MSG *msg = static_cast<MSG*>(message);
	if(msg->message != WM_NCHITTEST) return false;//只处理WM_NCHITTEST信息

	//lParam中的坐标原点是屏幕左上角
	int xPos = ((int)(short)LOWORD(msg->lParam)) - this->frameGeometry().x();
	int yPos = ((int)(short)HIWORD(msg->lParam)) - this->frameGeometry().y();

	if (xPos < 0 || yPos < 0 || xPos > this->width() || yPos > this->height())//不在窗体中
		return false;

	//在按钮组内
	if (ui->minBtn->geometry().contains(xPos, yPos)
			|| ui->closeBtn->geometry().contains(xPos, yPos))
		return false;
	//在标题栏内
	if (yPos < _titleHeight)
	{
		*result = HTCAPTION;
		return true;
	}

	return false;
}

//事件过滤器使滚轮失效
bool Login::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->userNameBox && event->type() == QEvent::Wheel)
    {
		return true;
    }
	return QWidget::eventFilter(obj, event);
}

