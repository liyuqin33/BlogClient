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
private:
    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
private slots:
    void actionLogin(); //显示响应
    void actionClose(); //关闭响应
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);   //单双击响应
signals:
    void show();//发出显示界面信号
    void close();//发出显示界面信号
};

#endif // TRAY_H
