#include "BlogEditArea.h"
#include "MyUtility.h"
#include "ui_BlogEditArea.h"
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QFileInfo>
#include <QDir>
#include <QWebFrame>
#include <QWebElement>
#include <QJsonDocument>
#include <QJsonArray>
#include <algorithm>
#include <QDebug>
#include <functional>

QString BlogEditArea::BLOG_FRAME = "";
QVector<int> BlogEditArea::OBSOLETE_ID_STORE;
int BlogEditArea::LOCAL_ID_BEGIN = 0;

void BlogEditArea::initBlogFrame()
{
	QTextStream in(&BLOG_FRAME, QIODevice::WriteOnly);
	//为什么不是默认？默认情况下，QTextStream 使用操作系统的本地编码进行读写。
    //不过你可以使用 setCodec() 函数进行设置
	in.setCodec("UTF-8");

	//可将文件内容存储为静态字符串，无需重复读取（注意内存！）
	QFile htmlFile(QStringLiteral(":/Data/blogSkim.html"));
	htmlFile.open(QIODevice::ReadOnly | QIODevice::Text);
	Q_ASSERT(htmlFile.isOpen());
	in << htmlFile.readAll();
	htmlFile.close();
}

QString BlogEditArea::articleHtmlFilePath(const QString &articleTitle)
{
	static const QString path = STORE_PATH() + "/%1.html";
	return path.arg(articleTitle);
}

QString BlogEditArea::articleHtmlFilePath(QModelIndex index)
{
	return articleHtmlFilePath("ID" +
				QString::number(index.data(CustomFilterModel::ID_ROLE()).toInt()));
}

BlogEditArea::BlogEditArea(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BlogEditArea),
	_editor(new EditView(this)),
	_blogHeadModel(new QStandardItemModel(this)),
	_proxyModel(new CustomFilterModel(this)),
	_isUserCreateArticle(false)
{
	ui->setupUi(this);
	ui->userFrame->hide();
	ui->editorContainLayout->addWidget(_editor);

	Q_ASSERT(!BLOG_FRAME.isEmpty());//必须先读取博客预览界面的框架

	//按钮（以后再改）
	QMenu *btnMenu = new QMenu(this);
	auto *newBlogAction = new QAction(QIcon(":/Image/NewBlog.png"), "新建文章", this);
	connect(newBlogAction, SIGNAL(triggered()), SLOT(userCreateArticle()));
	btnMenu->addAction(newBlogAction);

	auto *createCustomAction = new QAction(QIcon(":/Image/NewCustom.png"), "新建自定义分类", this);
	connect(createCustomAction, &QAction::triggered, this, &BlogEditArea::userCreateCustom);
	btnMenu->addAction(createCustomAction);

	ui->createBtn->setMenu(btnMenu);

	ui->blogHeadView->setDragEnabled(true);//使用QListView默认的拖拽!!!

	//读取用户信息
	readInfo();

	//模型
	_proxyModel->setSourceModel(_blogHeadModel);
	ui->blogHeadView->setModel(_proxyModel);

	connect(ui->customList, &QListWidget::currentItemChanged,
			this, &BlogEditArea::selectCustom);
	connect(ui->customList, &CustomListWidget::blogCustomChanged,
			_proxyModel, &CustomFilterModel::setFilterCustom);

	connect(ui->blogHeadView, &BlogHeadView::currentIndexChanged,
			this, &BlogEditArea::setCurrentArticle);//检查当前是否选择了文档

	//初始化状态
	ui->customList->setCurrentRow(0);
	_proxyModel->setFilterCustom(QString());
	setCurrentArticle(ui->blogHeadView->currentIndex());

	//背景
	hideEditor();

	ui->saveBtn->hide();
	ui->cancelBtn->hide();
}

BlogEditArea::~BlogEditArea()
{
	delete ui;
}

bool BlogEditArea::customReviewer(const QString &custom)
{
	auto list = ui->customList->findItems(custom, Qt::MatchFixedString);
	return list.isEmpty();
}

void BlogEditArea::closeEvent(QCloseEvent *event)
{
	if (saveInfo()) QWidget::closeEvent(event);
}

void BlogEditArea::readInfo()
{	
	//读取文件
	QFile blogInfoFile(BLOG_INFO_JSON_PATH());
	blogInfoFile.open(QIODevice::ReadOnly);
	if (!blogInfoFile.isOpen())
	{
		//QMessageBox::critical(this, "读取文件错误", "读取用户博客信息失败！");
		qDebug() << "没有读取到用户信息";
		return ;
	}
	QByteArray jsonData = blogInfoFile.readAll();
	blogInfoFile.close();

	//解析成json
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
	if (error.error != QJsonParseError::NoError)
	{
		QMessageBox::critical(this, "读取文件错误", "读取JSON格式文件错误！");
		return ;
	}

	QJsonObject root = doc.object();
	readIdInfo(root["BlogID"].toObject());
	readBlogInfo(root["BlogInfo"].toObject());
}

void BlogEditArea::readIdInfo(QJsonObject idRoot)
{
	Q_ASSERT(!idRoot["IdBegin"].isNull());
	Q_ASSERT(idRoot["ObsoleteIds"].isArray());

	LOCAL_ID_BEGIN = idRoot["IdBegin"].toInt();
	QJsonArray arr = idRoot["ObsoleteIds"].toArray();
	arr.begin();
	foreach (QJsonValue i, arr)
	{
		OBSOLETE_ID_STORE.append(i.toInt());
	}
}

void BlogEditArea::readBlogInfo(QJsonObject blogInfoRoot)
{
	for (auto arrIt = blogInfoRoot.constBegin(); arrIt != blogInfoRoot.constEnd(); ++arrIt)
	{
		//添加自定义分类
		QString custom = arrIt.key();
		if (!custom.isEmpty())
		{
			ui->customList->addItem(custom);
		}
		QJsonArray arr = arrIt.value().toArray();
		for (auto itemIt = arr.constBegin(); itemIt != arr.constEnd(); ++itemIt)
		{
			QJsonObject store = itemIt->toObject();
			QStandardItem *item = new QStandardItem(store["Title"].toString());
			item->setData(custom, CustomFilterModel::CUSTOM_ROLE());
			item->setData(store["IsPublish"].toBool(), CustomFilterModel::IS_PUBLISH_ROLE());
			item->setData(store["ID"].toInt(), CustomFilterModel::ID_ROLE());
			item->setData(store["Category"].toString(), CustomFilterModel::CATEGORY_ROLE());
			item->setData(store["Attached"].toString(), CustomFilterModel::ATTACHED_ROLE());
			item->setData(store["Tag"].toString().split(','), CustomFilterModel::TAG_ROLE());
			_blogHeadModel->appendRow(item);
		}
	}
}

bool BlogEditArea::saveInfo()
{
	QFile blogInfoFile(BLOG_INFO_JSON_PATH());
	blogInfoFile.open(QIODevice::WriteOnly);
	if (!blogInfoFile.isOpen())
	{
		return QMessageBox::Close ==
				QMessageBox::critical(this, "保存文件失败", "保存用户博客信息失败！",
									  QMessageBox::Close | QMessageBox::Cancel);
	}

	QJsonObject root;
	root.insert("BlogID", saveIdInfo());
	root.insert("BlogInfo", saveBlogInfo());

	QJsonDocument doc(root);
	blogInfoFile.write(doc.toJson());
	blogInfoFile.close();

	return true;
}

QJsonObject BlogEditArea::saveIdInfo()
{
	//缩减OBSOLETE_ID_STORE的长度
	std::sort(OBSOLETE_ID_STORE.begin(), OBSOLETE_ID_STORE.end());
	while (!OBSOLETE_ID_STORE.isEmpty() && OBSOLETE_ID_STORE.back() == LOCAL_ID_BEGIN - 1)
	{
		--LOCAL_ID_BEGIN;
		OBSOLETE_ID_STORE.pop_back();
	}

	QJsonArray arr;
	for (int i : OBSOLETE_ID_STORE) arr << i;
	QJsonObject idRoot;
	idRoot.insert("IdBegin", LOCAL_ID_BEGIN);
	idRoot.insert("ObsoleteIds", arr);

	return idRoot;
}

QJsonObject BlogEditArea::saveBlogInfo()
{
	//QJsonObject索引得到的是值类型，QMap索引得到的是引用类型，可以直接append，应该会更快？？？
	QMap<QString, QJsonArray> tempMap;

	//保存文章条目，使用QMap集中同种分类的文章
	auto parentItem = _blogHeadModel->invisibleRootItem();
	int blogCount = parentItem->rowCount();
	for (int i = 0; i != blogCount; ++i)
	{
		auto item = parentItem->child(i);
		QJsonObject storeItem;
		storeItem["IsPublish"] = item->data(CustomFilterModel::IS_PUBLISH_ROLE()).toBool();
		storeItem["ID"] = item->data(CustomFilterModel::ID_ROLE()).toInt();
		storeItem["Category"] = item->data(CustomFilterModel::CATEGORY_ROLE()).toString();
		storeItem["Attached"] = item->data(CustomFilterModel::ATTACHED_ROLE()).toString();
		storeItem["Tag"] = item->data(CustomFilterModel::TAG_ROLE()).toStringList().join(',');
		storeItem["Title"] = item->data(Qt::DisplayRole).toString();
		tempMap[item->data(CustomFilterModel::CUSTOM_ROLE()).toString()].append(storeItem);
	}

	QJsonObject blogInfoRoot;
	int customCount = ui->customList->count();
	for (int i = 1; i != customCount; ++i)//从1开始（忽略“全部”）
	{
		QString custom = ui->customList->item(i)->data(Qt::DisplayRole).toString();
		blogInfoRoot[custom] = tempMap[custom];//可能有空数组
	}
	blogInfoRoot[""] = tempMap[""];

	return blogInfoRoot;
}

int BlogEditArea::getFreeID()
{
	if (OBSOLETE_ID_STORE.isEmpty()) return LOCAL_ID_BEGIN++;
	int id = OBSOLETE_ID_STORE.back();
	OBSOLETE_ID_STORE.pop_back();
	return id;
}

void BlogEditArea::obsoleteID(int id)
{
	OBSOLETE_ID_STORE.push_back(id);
}

void BlogEditArea::setEditorVisible(bool isVisible)
{
	if (isVisible)
		showEditor();
	else
		hideEditor();
}

void BlogEditArea::showEditor()
{
	ui->bgLabel->hide();
	_editor->show();
}

void BlogEditArea::hideEditor()
{
	QSize s = _editor->size();
	_editor->hide();
	ui->bgLabel->resize(s);
	ui->bgLabel->show();
}

void BlogEditArea::editArticle(const QString &title)
{
	Article *article = _editor->getArticle();
	article->setTitle(title);

	if (title.isEmpty())
	{
		article->setContent(QString());
	}
	else//如果是编辑已存在的文章
	{
		QFile articleFile(articleHtmlFilePath(ui->blogHeadView->currentIndex()));
		articleFile.open(QIODevice::ReadOnly | QIODevice::Text);
		Q_ASSERT(articleFile.isOpen());
		QString pageHtml = articleFile.readAll();//读取HTML
		articleFile.close();

		QString headerStr("<div class=\"articleContent\">");
		int beg = pageHtml.indexOf(headerStr) + headerStr.size();//内容开头
		QRegExp endRegExp("</div>\\s+<div class=\"categoryCount\">");
		int end = endRegExp.indexIn(pageHtml);//内容结尾
		article->setContent(pageHtml.mid(beg, end - beg));//设置内容
	}

	ui->blogInfoFrame->hide();
	showEditor();
	ui->editBtn->hide();
	ui->saveBtn->show();
	ui->cancelBtn->show();

	//当editor读取编辑页面时，网页中的Javascript函数（自定义，在postArticle.js）
	//会使用_editor->article来初始化网页中文章的标题和内容
	_editor->load(POST_ARTICLE_HTML_URL());
}

void BlogEditArea::saveArticle(Article *article)
{
	//停止正在阅读的页面，可能就是当前编写的页面（会崩溃？有必要停止吗？）
	_editor->stop();

	//假定正在编写的文件是当前文件！！！
	_proxyModel->setData(ui->blogHeadView->currentIndex(),
									   article->title(), Qt::DisplayRole);
	//保存为HTML文件，方便WebView直接Load？？？
	QFile targetHtmlFile(articleHtmlFilePath(ui->blogHeadView->currentIndex()));
	targetHtmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
	Q_ASSERT(targetHtmlFile.isOpen());
	QTextStream out(&targetHtmlFile);
	out.setCodec("UTF-8");//为什么不是默认？

	out << BLOG_FRAME.arg(article->title()).arg(article->content());

	targetHtmlFile.close();
}

void BlogEditArea::userCreateArticle()
{
	_isUserCreateArticle = true;//必须先于setCurrentIndex(changed)

	QStandardItem *articleItem = new QStandardItem();//文章条目

	if (ui->customList->currentRow() != 0)//“全部”是第0个条目，不算分类
	{
		QListWidgetItem *customItem = ui->customList->currentItem();
		Q_ASSERT(customItem != nullptr);//必须选中一种（有可能什么都没选中吗？）
		articleItem->setData(customItem->data(Qt::DisplayRole), CustomFilterModel::CUSTOM_ROLE());
	}
	articleItem->setData(getFreeID(), CustomFilterModel::ID_ROLE());//获取独特ID
	articleItem->setData(false, CustomFilterModel::IS_PUBLISH_ROLE());//未发布
	_blogHeadModel->appendRow(articleItem);
	_proxyModel->invalidate();
	QModelIndex index = _blogHeadModel->indexFromItem(articleItem);
	index = _proxyModel->mapFromSource(index);
	ui->blogHeadView->setCurrentIndex(index);
	editArticle();
}

void BlogEditArea::userCreateCustom()
{
	QString custom = QInputDialog::getText(this, "创建自定义分类", "请输入分类名称");
	if (custom.isEmpty()) return;
	if (!customReviewer(custom))
	{
		QMessageBox::warning(this, "创建分类错误", "请输入合法的分类名称");
		return ;
	}
	new QListWidgetItem(custom, ui->customList);
}

void BlogEditArea::setCurrentArticle(const QModelIndex &proxyIndex)
{
	if (_isUserCreateArticle) return ;//简单屏蔽
	if (!proxyIndex.isValid())
	{
		ui->editBtn->setEnabled(false);
		hideEditor();
		return ;
	}

	ui->editBtn->setEnabled(true);
	showEditor();
	//打开保存文件
	QString articlePath = articleHtmlFilePath(ui->blogHeadView->currentIndex());
	Q_ASSERT(QFileInfo::exists(articlePath));
	_editor->load(QUrl::fromLocalFile(articlePath));
/*
 *	setHtml会更快吗？
 *
	QFile articleFile(articlePath);
	articleFile.open(QIODevice::ReadOnly | QIODevice::Text);
	Q_ASSERT(articleFile.isOpen());
	QString pageHtml = articleFile.readAll();
	articleFile.close();

	_editor->setHtml(pageHtml, QUrl::fromLocalFile(STORE_PATH() + "/b"));
*/
}

//更新用户浏览的文章——更新分类，（选择“全部”时更新为空字符串）
void BlogEditArea::selectCustom(QListWidgetItem *currentItem)
{
	Q_ASSERT_X(currentItem != nullptr, "In BlogEditArea::category",
			   "CurrentItem == nullptr!失焦？");
	_proxyModel->setFilterCustom(ui->customList->row(currentItem) != 0 ?
				currentItem->data(Qt::DisplayRole).toString()
			  : QString());
}

void BlogEditArea::on_editBtn_clicked()
{
	QModelIndex index = ui->blogHeadView->currentIndex();
	editArticle(index.data().toString());
}

void BlogEditArea::on_cancelBtn_clicked()
{
	_isUserCreateArticle = false;

	QModelIndex index = ui->blogHeadView->currentIndex();
	if (index.data().toString().isEmpty())
	{
		QFile::remove(articleHtmlFilePath(index));
		obsoleteID(index.data(CustomFilterModel::ID_ROLE()).toInt());
		ui->blogHeadView->model()->removeRow(index.row(), index.parent());
	}

//	_editor->load(QUrl::fromLocalFile(articleHtmlFilePath(ui->blogHeadView->currentIndex())));
	ui->cancelBtn->hide();
	ui->saveBtn->hide();
	ui->editBtn->show();
	ui->blogInfoFrame->show();
	setCurrentArticle(ui->blogHeadView->currentIndex());
}

void BlogEditArea::on_saveBtn_clicked()
{
	//调用网页里JavaScript的一个函数（在postArticle.js）
	//它是我自己写的，它将文章的标题、内容赋值到article.title和article.content
	_editor->page()->mainFrame()->evaluateJavaScript("updateArticle()");
	//看到输出之后就会清楚前端发送的数据是怎样的HTML格式的数据
	qDebug() << "Title " << _editor->getArticle()->title() << endl
			 << "Content " << _editor->getArticle()->content();

	if (_editor->getArticle()->title().isEmpty())
	{
		QMessageBox::warning(this, "标题不能为空", "文章标题不能为空！");
		return ;
	}

	saveArticle(_editor->getArticle());
}
