#include "login.h"
#include "ui_login.h"
#include <QDebug>

Login::Login(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Login)
{
    _isLeftMouseBtnPressed = false;
    initUI();//加载UI
    initTray();//加载系统托盘
    loadUser();//加载本地用户
    initLink();//加载信号
}

Login::~Login()
{
    delete ui;
}


void Login::initUI()//加载UI
{
    //设置窗口
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::FramelessWindowHint;
    setFixedSize(300,350);
    setWindowFlags(flags);

    //ui
    ui->setupUi(this);
    ui->loginWidget->show();

    //qss
    QFile qss(":/default.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
}


void Login::initTray()//加载系统托盘
{
    /*qsystemtrayicon*/
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
}

//槽函数
void Login::clickCloseBtn()//点击关闭响应函数
{
    close();
}

void Login::clickMinBtn()//点击最小化响应函数
{
    this->showMinimized();
}

void Login::login()//点击登陆槽函数
{
    emit logining(ui->userNameBox->lineEdit()->text(), ui->passwordEdit->text());
    qDebug()<<"登陆界面发出登陆信号"<<ui->userNameBox->lineEdit()->text()<<ui->passwordEdit->text();

    isEnter(true);//该句调试用
}
void Login::registerUser()//点击注册槽函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
}

void Login::findPasswords()//点击找回密码槽函数
{
    /*此处添加跳转网页*/
    const QUrl url("http://www.baidu.com");
    qDebug() << url.scheme();
    qDebug() << url.port();
    QDesktopServices::openUrl(url);
}


void Login::isEnter(bool isEnter)//后台返回是否能登陆对接的槽函数
{
    if(isEnter)
    {
        addUser();
        done();
    }
    else
        ui->hintLabel->setText("密码错误");
}

void Login::done()//结束时的槽函数（登陆并关闭）
{
    QDialog::accept();
}

void Login::addUser()//添加本地用户
{
    /*此处应把成功登陆用户存于本地*/
}

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
