#include "accountitem.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

AccountItem::AccountItem(QWidget *parent, QListWidgetItem *item,
						 QString ID, QString password, bool isSavePassword)
	: QWidget(parent)
	, _label(new QLabel(ID))
	, _item(item)
	, _password(password)
	, _isSavePassword(isSavePassword)
{
	//初始化
	setObjectName("accountItem");
	_isMousePress = false;
	//删除按钮
	_deleteBtn = new QPushButton();
	_deleteBtn->setObjectName("deleteBtn");
	QPixmap pixmap(":/Image/login/item_del.png");
	_deleteBtn->setIcon(pixmap);
	_deleteBtn->setFixedSize(16, 16);//图片周围有空白，将删除按钮与下拉按钮对齐
	connect(_deleteBtn, &QPushButton::clicked, this, &AccountItem::tryRemoveAccount);
	//布局
	_layout = new QHBoxLayout(this);
	_layout->addWidget(_label);
	_layout->addStretch();
	_layout->addWidget(_deleteBtn);
//	_layout->setSpacing(5);目的是？
	_layout->setContentsMargins(5, 5, 0, 5);//将删除按钮与下拉按钮对齐
	setLayout(_layout);
}
AccountItem::~AccountItem(){}

//设置和获取
void AccountItem::setID(QString account_text)    //设置用户ID
{
	_label->setText(account_text);
}
void AccountItem::setPassword(QString passwords)    //设置用户密码
{
	_password=passwords;
}
void AccountItem::setIsSavePassword(bool isAuto)    //设置用户自动登陆
{
	_isSavePassword=isAuto;
}

QString AccountItem::getID()    //获取用户ID
{
	return _label->text();
}
QString AccountItem::getPassword()    //获取用户密码
{
	return _password;
}
bool AccountItem::getIsSavePassword()    //获取用户自动登陆
{
	return _isSavePassword;
}

//响应函数
void AccountItem::tryRemoveAccount()   //移除响应函数
{
	emit removeAccount(_item);
}
void AccountItem::choose()  //选择响应函数
{
	emit showAccount(_item);
}

//鼠标事件
void AccountItem::mousePressEvent(QMouseEvent *event)    //鼠标按下事件重写
{
	if(event->button() == Qt::LeftButton)
	{
		_isMousePress = true;
	}
}
void AccountItem::mouseReleaseEvent(QMouseEvent *)    //鼠标释放事件重写
{
	if(_isMousePress)
	{
		emit showAccount(_item);
		_isMousePress = false;
	}
}

