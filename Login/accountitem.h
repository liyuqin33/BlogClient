#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>

class QPushButton;
class QLabel;
<<<<<<< HEAD
class QHBoxLayout;
=======
>>>>>>> refs/heads/pr/9
class QListWidgetItem;


class AccountItem:public QWidget    //项目
{
	Q_OBJECT
public:
	AccountItem(QWidget *parent, QListWidgetItem *item,
				QString ID, QString password, bool isSavePassword);
	~AccountItem();
	void setID(QString);    //设置用户ID
	void setPassword(QString);    //设置用户密码
	void setIsSavePassword(bool);    //设置用户自动登陆
	QString getID();    //获取用户ID
	QString getPassword();    //获取用户密码
	bool getIsSavePassword();    //获取用户自动登陆
private:
	//布局
	QPushButton* _deleteBtn;
<<<<<<< HEAD
	QLabel* _label;
	QHBoxLayout* _layout;
=======
    QLabel* _label;
>>>>>>> refs/heads/pr/9
	//数据
	QListWidgetItem *_item;
	QString _password;
	bool _isSavePassword;
	//判断
	bool _isMousePress;
public slots:
	void tryRemoveAccount();   //移除响应函数
	void choose();  //选择响应函数
protected:
	void mousePressEvent(QMouseEvent *);    //鼠标按下事件重写
	void mouseReleaseEvent(QMouseEvent *);  //鼠标释放事件重写
signals:
	void showAccount(QListWidgetItem *item);   //发出显示信号
	void removeAccount(QListWidgetItem *item);     //发出移除信号
};

#endif // ACCOUNTITEM_H
