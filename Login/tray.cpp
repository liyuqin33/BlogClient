#include "tray.h"
#include <QMenu>

Tray::Tray(QWidget* parent)
    :QWidget(parent)
{
    //托盘菜单
    _trayIconMenu = new QMenu(parent);
    QAction* loginAction=new QAction(QIcon(""), "登录", parent);
    connect(loginAction, SIGNAL(triggered()), this, SLOT(actionLogin()));
    _trayIconMenu->addAction(loginAction);
    QAction* closeAction=new QAction(QIcon(""), "关闭", parent);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(actionClose()));
    _trayIconMenu->addAction(closeAction);
    //系统托盘
    _trayIcon=new QSystemTrayIcon(parent);
	_trayIcon->setIcon(QIcon(":/Image/login/blog_unlogin.png"));
    _trayIcon->setContextMenu(_trayIconMenu);
    _trayIcon->show();
    connect(_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    _trayIcon->setToolTip(tr("研发博客"));
    _trayIcon->showMessage(tr("研发博客"), tr("研发博客登陆"), QSystemTrayIcon::Information, 5000);//在系统托盘显示气泡消息提示
}
void Tray::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        //点击托盘显示窗口
        case QSystemTrayIcon::Trigger:
        {
            actionLogin();
            break;
        }
        //双击托盘显示窗口
        case QSystemTrayIcon::DoubleClick:
        {
            actionLogin();
            break;
        }
        default:
            break;
    }
}
Tray::~Tray()
{
    delete _trayIconMenu;
    delete _trayIcon;
}
void Tray::actionLogin()
{
    emit show();
}
void Tray::actionClose()
{
    emit close();
}
