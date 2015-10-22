#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFile>
#include <QUrl>
#include <QDesktopServices>
#include <QMouseEvent>
#include <QPoint>

#define TITLE_HEIGHT 25

namespace Ui {
class Login;
}

class Login: public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
private:
    //UI
    Ui::Login *ui;

    //加载
    void initUI();//加载UI    
    void initTray();//加载系统托盘
    void loadUser();//加载本地用户
    void initLink();//加载信号

    //拖动数据
    bool    _isLeftMouseBtnPressed;//是否按下
    QPoint  _ptLast;    //按下位置
public slots:
    //用户交互
    void login();//点击登录响应函数
    void registerUser();//点击注册响应函数
    void findPasswords();//点击找回密码响应函数
    void clickCloseBtn();//点击关闭响应函数
    void clickMinBtn();//点击最小化响应函数

    //后台交互
    void isEnter(bool); //判断是否登陆

    //结束
    void done();//结束时的槽函数（登陆并关闭）
    void addUser();//登陆添加本地用户
signals:
    void logining(QString,QString);//发出登陆信号
protected:
    void mouseMoveEvent(QMouseEvent *event);         //重写鼠标移动事件
    void mousePressEvent(QMouseEvent *event);        //重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *event);     //重写鼠标松开事件
};

#endif // LOGIN_H
