#ifndef BLOGEDITAREA_H
#define BLOGEDITAREA_H

#include "CustomFilterModel.h"
#include "WebCorrespond/article.h"
#include "editview.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QFile>
#include <QJsonObject>
/*class BlogEditArea
 * 文章分类共有两种，系统定义的分类和用户定义的分类
 *
 * 系统定义的分类是用来在社区里筛选文章的，所以是强制的，并且共有两层分类，称第一层分类为category，第二层为attached
 *
 * 用户定义的分类是便于用户自己管理设置的，所以是不必要的，只有一层分类，不属于用户定义的任何一个分类的文章将在“全部”分类中找到，
 * 将用户定义的分类称为Custom
 *
 * CustomList->model() == _proxyModel
 *
 * 文章靠自身独特的ID来辨别，因为前端允许同类同名
 * ID分两种，一种是客户端分发的ID，在文章没有发布时使用，另一种是后台返回的ID，在文章发布后使用
 * 文章是否发布存储在blogItem->data(false, CustomFilterModel::IS_PUBLISH_ROLE());
*/

namespace Ui {
class BlogEditArea;
}

class BlogEditArea : public QWidget
{
	Q_OBJECT

public:
	static void initBlogFrame();
	static QString articleHtmlFilePath(const QString &articleTitle);
	static QString articleHtmlFilePath(QModelIndex index);

	explicit BlogEditArea(QWidget *parent = nullptr);
	~BlogEditArea();
	bool customReviewer(const QString &custom);

	EditView* editView(){return _editView;}
protected:
	void closeEvent(QCloseEvent *event)override;

private slots:
	//用户主动创建文章
	void userCreateArticle();

	//用户主动创建自定义分类
	void userCreateCustom();

	//将proxyIndex表示的文章设置为当前文章
	void setCurrentArticle(const QModelIndex &proxyIndex);

	//将currentItem表示的自定义分类设置成当前的自定义分类
	void selectCustom(QListWidgetItem *currentItem);

	//当编辑按钮按下时，进入编辑状态
	void on_editBtn_clicked();

	//当退出编辑按钮按下时，退出编辑状态
	void on_cancelBtn_clicked();

	//当保存按钮按下时，保存文章
	void on_saveBtn_clicked();

private:
	//读取本地信息
	void readInfo();
	void readIdInfo(QJsonObject idRoot);
	void readBlogInfo(QJsonObject blogInfoRoot);
	//保存本地信息
	bool saveInfo();
	QJsonObject saveIdInfo();
	QJsonObject saveBlogInfo();

	//获取/弃用ID
	int getFreeID();
	void obsoleteID(int id);

	//设置编辑窗口是否可见
	//当没有选择文章时，显示背景Label
	void setEditorVisible(bool isVisible);
	void showEditor();
	void hideEditor();

	//保存文章
	void saveArticle(Article *article);

	//编辑文章
	void editArticle(const QString &title = QString());
//Field
private:
	static QString BLOG_FRAME;//阅读页面外观的HTML代码
	static QVector<int> OBSOLETE_ID_STORE;//返回的ID临时缓存区
	static int LOCAL_ID_BEGIN;//新ID的起始位置

	Ui::BlogEditArea *ui;
	EditView *_editView;
	QStandardItemModel *_blogHeadModel;
	CustomFilterModel *_proxyModel;
	bool _isUserCreateArticle;//新建文件时会触发BlogHeadView的currentIndexChanged的信号，简单屏蔽一下
};

#endif // BLOGEDITAREA_H
