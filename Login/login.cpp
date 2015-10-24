//托盘类
#include <QDesktopWidget>
#include <QDesktopServices>
//Login类
#include "login.h"
#include "accountitem.h"
#include "MyUtility.h"
#include <QDialog>
#include <QFile>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>
#include <QPoint>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
//调试类
#include <QDebug>

Login::Login(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Login)
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
}
void Login::initUI()//加载UI
{
    //设置窗口
    setFixedSize(300,350);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);

    //ui
    ui->setupUi(this);
    ui->loginWidget->show();

    //qss
    QFile file(":/login.qss");
    file.open(QFile::ReadOnly);
	Q_ASSERT(file.isOpen());
    QString styleSheet = file.readAll();
    this->setStyleSheet(styleSheet);
}
void Login::initTray()//加载系统托盘
{
	_tray=new Tray((QWidget*)this->parent());
    connect(_tray, SIGNAL(show()), this, SLOT(clickShowBtn()));
    connect(_tray, SIGNAL(close()), this, SLOT(clickCloseBtn()));
}

void Login::initLink()//加载信号
{
    connect(ui->loginBtn, SIGNAL(clicked()), this, SLOT(login()));//登陆
    connect(ui->findPasswordsBtn, SIGNAL(clicked()), this, SLOT(findPasswords()));//找回密码
    connect(ui->registerBtn, SIGNAL(clicked()), this, SLOT(registerUser()));//注册
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(clickCloseBtn()));
	connect(ui->minBtn, SIGNAL(clicked()), this, SLOT(clickMinBtn()));
}

void Login::loadUser()//加载本地用户
{
    /*此处应把存于本地的用户载入框并判断是否登陆*/
    _listWidget = new QListWidget(this);
    ui->userNameBox->setEditable(true);
    ui->userNameBox->setModel(_listWidget->model());
    ui->userNameBox->setView(_listWidget);
    //读取文件
    QFile blogInfoFile(USER_INFO_JSON_PATH());
    blogInfoFile.open(QIODevice::ReadOnly);
    if (!blogInfoFile.isOpen())
    {
        QMessageBox::critical(this, "读取文件错误", "读取用户博客信息失败！");
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
        QMessageBox::critical(this, "读取文件错误", "读取JSON格式文件错误！");
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
			ui->loginBtn->animateClick();
		}
	}
}

void Login::clickShowBtn()//点击恢复响应函数
{
    setFixedSize(300,350);
}
void Login::clickCloseBtn()//点击关闭响应函数
{
    close();
}
void Login::clickMinBtn()//点击最小化响应函数
{
    setFixedSize(0,0);
}
void Login::login()//点击登陆响应函数
{
    emit logining(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text());
    qDebug()<<"登陆界面发出登陆信号"<<ui->userNameBox->lineEdit()->text()<<ui->passwordEdit->text();

    isEnter(true);//该句调试用
}
void Login::registerUser()//点击注册响应函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
}
void Login::findPasswords()//点击找回密码响应函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
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
		if (updateItem)
		{
			AccountItem *firstAccountWidget = (AccountItem*)_listWidget->itemWidget(_listWidget->item(0));
			ui->userNameBox->setEditText(firstAccountWidget->getID());
			ui->passwordEdit->setText(firstAccountWidget->getPassword());
			ui->savePasswordBox->setChecked(firstAccountWidget->getIsSavePassword());
		}
	}
}

//后台交互
void Login::isEnter(bool isEnter)//后台返回是否能登陆对接的槽函数
{
    if(isEnter)
    {
        addUser();
		this->Login::done();
    }
    else
        ui->hintLabel->setText("密码错误");
}

//结束
void Login::done()//结束时的槽函数（登陆并关闭）
{
    QDialog::done(QDialog::Accepted);
}

void Login::addUser()//添加本地用户
{
    /*此处应把成功登陆用户存于本地*/
    QVariantList userList;
	QString user = ui->userNameBox->lineEdit()->text();
    int i;
	bool isExist=false;
    for(i=0; i<_listWidget->count(); i++)//判断用户是否重复
    {
		if(((AccountItem*)_listWidget->itemWidget(_listWidget->item(i)))->getID() == user)//如果相同
			//重复替换原来的并顶置
        {
			isExist=true;
            //替换
			//takeItem会删除item上的AccountWidget,所以要new一个
			AccountItem *replaceAccount = new AccountItem(this, _listWidget->item(i)
									, ui->userNameBox->lineEdit()->text()
									, ui->passwordEdit->text()
									, ui->savePasswordBox->isChecked());
            //顶置
			auto item = _listWidget->takeItem(i);
			_listWidget->insertItem(0, item);
			_listWidget->setItemWidget(item, replaceAccount);
            break;
        }
    }
    //生成json
	if(!isExist)//不重复在json添加一个用户
    {
        QVariantMap jsonItem;
		saveAccount(jsonItem,
					ui->userNameBox->lineEdit()->text(),
					ui->passwordEdit->text(),
					ui->savePasswordBox->isChecked());
		userList << jsonItem;
    }
    for(int i=0; i<_listWidget->count(); i++)
    {
        QVariantMap jsonItem;
		AccountItem *account = (AccountItem*)_listWidget->itemWidget(_listWidget->item(i));
		saveAccount(jsonItem, account->getID(), account->getPassword(), account->getIsSavePassword());
		userList << jsonItem;
    }
    QVariantMap all;
    all.insert("UserInfo", userList);
	all.insert("AutoLogin", ui->autoLoginBox->isChecked());
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

void Login::on_autoLoginBox_clicked(bool checked)
{
	if (checked) ui->savePasswordBox->setChecked(checked);//自动登陆必须记住密码（不会发出信号）
}
