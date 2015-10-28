#include "tray.h"
#include <QMenu>


#include <QDebug>
#include <QFile>


Tray::Tray(QWidget* parent)
    :QWidget(parent)
{
    initUnlogn();
}
Tray::~Tray()
{
    delete _trayIconMenu;
    delete _trayIcon;
}

void Tray::initUnlogn()
{
    //托盘菜单
    _trayIconMenu = new QMenu((QWidget*)this->parent());

    _trayIconMenu->setObjectName("trayMenu");

    QAction* loginAction=new QAction(QIcon(""), "登录", (QWidget*)this->parent());
    connect(loginAction, SIGNAL(triggered()), this, SLOT(actionLogin()));
    _trayIconMenu->addAction(loginAction);
    QAction* closeAction=new QAction(QIcon(""), "关闭", (QWidget*)this->parent());
    connect(closeAction, SIGNAL(triggered()), this, SLOT(actionClose()));
    _trayIconMenu->addAction(closeAction);
    //系统托盘
    _trayIcon=new QSystemTrayIcon((QWidget*)this->parent());
    _trayIcon->setIcon(QIcon(":/Image/login/blog_unlogin.png"));
    _trayIcon->setContextMenu(_trayIconMenu);
    _trayIcon->show();

    connect(_trayIcon, &QSystemTrayIcon::activated, this, &Tray::iconIsActived);
    _trayIcon->setToolTip(tr("研发博客"));
}

//公有函数
void Tray::showHit(const QString & title, const QString & str)//在系统托盘显示气泡消息提示
{
    _trayIcon->showMessage(title, str, QSystemTrayIcon::Information, 5000);
}


void Tray::logned()//功能未完善
{
    //托盘菜单
    _trayIconMenu->clear();

    QAction* showAction=new QAction(QIcon(""), "显示主窗口", (QWidget*)this->parent());
    connect(showAction, SIGNAL(triggered()), this, SLOT(actionShow()));
    _trayIconMenu->addAction(showAction);
    QAction* setAction=new QAction(QIcon(""), "设置", (QWidget*)this->parent());
    connect(setAction, SIGNAL(triggered()), this, SLOT(actionSet()));
    _trayIconMenu->addAction(setAction);

    QAction* logoutAction=new QAction(QIcon(""), "注销", (QWidget*)this->parent());
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(actionLogout()));
    _trayIconMenu->addAction(logoutAction);
    QAction* closeAction=new QAction(QIcon(""), "退出", (QWidget*)this->parent());
    connect(closeAction, SIGNAL(triggered()), this, SLOT(actionClose()));
    _trayIconMenu->addAction(closeAction);
    //系统托盘
    _trayIcon->setIcon(QIcon(":/Image/login/blog_logined.png"));
    _trayIcon->setContextMenu(_trayIconMenu);
    _trayIcon->show();
}

//私有响应函数
void Tray::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{

    /*
     * ???当窗口未缩下，信号发出不能顶置窗口
     */

    switch(reason)
    {
        //点击托盘显示窗口
        case QSystemTrayIcon::Trigger:
        {

            qDebug()<<"单击";

            actionLogin();
            break;
        }
        //双击托盘显示窗口
        case QSystemTrayIcon::DoubleClick:
        {

            qDebug()<<"双击";

            actionLogin();
            break;
        }
        default:
            break;
    }
}
void Tray::actionLogin()
{
    emit showWindow();
}
void Tray::actionClose()
{
    emit closeWindow();
}
void Tray::actionShow()
{
    emit showWindow();
}
void Tray::actionSet()
{
    emit set();
}
void Tray::actionLogout()
{
    emit logout();
}
