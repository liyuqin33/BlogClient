#ifndef LOGIN_H
#define LOGIN_H


#define PI 3.1415926535897932

#include "ui_login.h"
#include "tray.h"
#include <QWidget>

//接口数据类
class QFile;
class QUrl;
//布局事件类
class QListWidget;
class QListWidgetItem;
class AccountItem;
class QLabel;
class QMouseEvent;
//位置时间动画类
class QTimeLine;
class QMovie;


namespace Ui {
class Login;
}

class Login: public QWidget
{
    Q_OBJECT
private:

    //!返回信号类型
	enum LoginMessage
	{
		LOG_IN,
		ERROR_PASSWORD,
		ERROR_USER,
		ERROR_UNNET
	};
private:

	//! 组件
    Ui::Login *ui;
	QListWidget* _listWidget;
	//! 判断数据
    bool    _isLeftMouseBtnPressed;//!是否按下
    bool    _isOldAutoLogin;//!是否加载时为自动登录
    QPoint  _ptLast;    //!按下位置
	//! 登陆中数据
    QTimeLine *_timeLine;
    QWidget *_headWidget;
    QLabel *_headPix;
    QLabel *_headLine;
    QMovie *_headLineMovie;

	int _titleHeight;
    //!控制数据
    bool _isFlash;  //!是否要渐变动画
public:
	explicit Login(QWidget *parent = nullptr, bool isFlash = true);
	~Login();
public slots:
//接口
	//! 显示窗口
	void clickShowBtn();
	//! 关闭窗口
	void clickCloseBtn();
//用户交互-QComboBox
	//! 将选项文本显示在QComboBox当中
	void showAccount(QListWidgetItem *item);
	//! 删除帐号
	void removeAccount(QListWidgetItem *item);
//后台交互
	//! 判断是否登陆
	void checkLoginMessage(LoginMessage type);
signals:
	//! 发出登陆信号
	void tryLogin(QString user,QString password,bool isEncryption);
	//! 取消登录信号
    void unLogining();
	//! 成功登陆信号
	void successfulLogin();
	//!
	void sendMessage(QString message);
protected:
	/*!
	 * \brief nativeEvent
	 * \param eventType
	 * \param message
	 * \param result
	 * \return true表示该事件已处理，不需要Qt再进行处理；false表示该事件需要Qt进行处理
	 *
	 * 只有标题栏需要处理(return true)，其他情况都由Qt处理(return false)
	 */
	bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;
	/*!
	 * \brief 过滤滚轮事件
	 * \param obj
	 * \param event
	 * \return
	 */
	bool eventFilter(QObject *obj, QEvent *event);
private slots:
	//! 用户交互
	/*!
	 * \brief userTryLogin 登录响应函数
	 * \param isDown
	 */
	void userTryLogin(bool isDown);

	/*!
	 * \brief registerUser 注册响应函数
	 */
	void registerUser();

	//!点击找回密码响应函数
	void findPassword();

	//!点击最小化响应函数
	void clickMinBtn();

	//!键盘修改用户名称
	void changUser(const QString &editUser);
	/*!
	 * \brief premiseSavePassword 自动登陆必须记住密码
	 * \param isAutoLogin 是否自动登陆
	 * \warning savePasswordBtn不会发出clicked信号
	 */

	void premiseSavePassword(bool isAutoLogin);

	//! 登陆时交互

	//!登陆中的时间响应
	void timesUp(int);
private:
	//! 加载

	//! 初始化数据
	void initData();
	//! 加载UI
	void initUI();
	//! 加载本地用户
	void loadUser();
	//! 加载信号
	void initLink();

	/*!
	 * \brief 保存账号
	 * \param item 输出的账号
	 * \param id 账号名
	 * \param password 账号密码
	 * \param isSavePassword 是否保存密码
	 *
	 * 如果没有设置保存密码，将不会保存密码
	 * 下次读取的密码就会为空，所以可以直接设置passwordBox，无需判断
	 */
	void saveAccount(QVariantMap &item,
					 const QString &id,
					 const QString &password,
                     bool isSavePassword);

	//! 添加登陆成功的用户
	void addCurrentUser();
	//! 保存listWidget记录的用户
	void saveUser(bool checkLoginMessage);
	//! 显示正在登陆界面
    void showLoginInterface();
	//! 结束正在登陆界面
    void endLoginInterface();
};

#endif // LOGIN_H
