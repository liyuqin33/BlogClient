#ifndef LOGIN_H
#define LOGIN_H

#include "ui_login.h"
#include "tray.h"
#include <QDialog>
#include <QVector>

class QFile;
class QUrl;
class QMouseEvent;
class QPoint;
class QListWidget;
class QListWidgetItem;
class AccountItem;

namespace Ui {
class Login;
}

class Login: public QDialog
{
    Q_OBJECT
private:
	//返回信号类型
	enum Load
	{
		LOG_IN,
		ERROR_PASSWORD,
		ERROR_USER,
		ERROR_UNNET
	};
private:
	//组件
    Ui::Login *ui;
	QListWidget* _listWidget;
    //判断数据
    bool    _isLeftMouseBtnPressed;//是否按下
    QPoint  _ptLast;    //按下位置


public:
	explicit Login(QWidget *parent = 0);
	~Login();
    Tray *_tray;
public slots:
	//用户交互-QComboBox
	void showAccount(QListWidgetItem *item);//将选项文本显示在QComboBox当中
	void removeAccount(QListWidgetItem *item);//删除帐号时
	//后台交互
	void isSuccessfulLoaded(Load type); //判断是否登陆
	//结束
	void done();//结束时的槽函数（登陆并关闭）
signals:
	void logining(QString user,QString password,bool isEncryption);//发出登陆信号
protected:
	void mouseMoveEvent(QMouseEvent *event);         //重写鼠标移动事件
	void mousePressEvent(QMouseEvent *event);        //重写鼠标按下事件
	void mouseReleaseEvent(QMouseEvent *event);     //重写鼠标松开事件

private slots:
	//用户交互
	void login();//点击登录响应函数
	void registerUser();//点击注册响应函数
	void findPassword();//点击找回密码响应函数
	void clickCloseBtn();//点击关闭响应函数
	void clickShowBtn();//点击恢复响应函数
	void premiseSavePassword(bool);
private:
	//加载
	void initData();//初始化数据
	void initUI();//加载UI
	void initTray();//加载系统托盘
	void loadUser();//加载本地用户
	void initLink();//加载信号

	/*
	 * 目的：保存账号
	 * 注意：如果没有设置保存密码，将不会保存密码
	 *		下次读取的密码就会为空，所以可以直接设置passwordBox，无需判断
	*/
	void saveAccount(QVariantMap &item,
					 const QString &id,
					 const QString &password,
                     bool isSavePassword);
	/*
	 * 目的：添加登陆成功的用户
	*/
	void addCurrentUser();
	/*
	 * 目的：保存listWidget记录的用户
	*/
	void saveUser();

	/*
	 * 目的：显示正在登陆界面
	*/
	void showLoginInterface();
};

#endif // LOGIN_H
