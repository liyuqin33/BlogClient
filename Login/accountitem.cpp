﻿#include "accountitem.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

AccountItem::AccountItem(QWidget *parent, QListWidgetItem *item,
                         QString ID, QString password, QString pixURL, bool isSavePassword)
	: QWidget(parent)
    , _label(new QLabel(ID))
	, _item(item)
	, _password(password)
    , _pixURL(pixURL)
	, _isSavePassword(isSavePassword)
{
	//初始化
	setObjectName("accountItem");
	_isMousePress = false;
	//删除按钮
    _deleteBtn = new QPushButton(this);
	_deleteBtn->setObjectName("deleteBtn");
	QPixmap pixmap(":/Image/login/item_del.png");
	_deleteBtn->setIcon(pixmap);
	_deleteBtn->setFixedSize(16, 16);//图片周围有空白，将删除按钮与下拉按钮对齐
	connect(_deleteBtn, &QPushButton::clicked, this, &AccountItem::tryRemoveAccount);
    //布局
    _label->setParent(this);
    _label->setGeometry(5,0,170, 30);
    _deleteBtn->setGeometry(175,7,16, 16);//将删除按钮与下拉按钮对齐
}
AccountItem::~AccountItem(){}

//设置和获取
void AccountItem::setID(QString account_text)    //设置用户ID
{
	_label->setText(account_text);
}
void AccountItem::setPassword(QString password)    //设置用户密码
{
    _password=password;
}
void AccountItem::setPixURL(QString pixURL)    //设置用户头像
{
    _pixURL=pixURL;
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
QString AccountItem::getPixURL()    //获取用户头像
{
    return _pixURL;
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
