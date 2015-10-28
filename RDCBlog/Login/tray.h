#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>
#include <QWidget>

class QMenu;


class Tray:public QWidget
{
    Q_OBJECT
public:
    Tray(QWidget *parent);
    ~Tray();

    void showHit(const QString &title, const QString &str);
private:
    void initUnlogn();

    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
public slots:
    void logned();//已登陆响应
private slots:
    void actionLogin(); //登陆响应
    void actionClose(); //关闭响应
    void actionShow(); //显示响应
    void actionSet(); //设置响应
    void actionLogout(); //注销响应
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);   //单双击响应
signals:
    void showWindow();//发出显示界面信号
    void closeWindow();//发出显示界面信号
    void set();//发出设置信号
    void logout();//发出登出信号
};

#endif // TRAY_H