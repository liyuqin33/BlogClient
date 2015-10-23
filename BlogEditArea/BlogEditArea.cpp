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

BlogEditArea::BlogEditArea(QWidget *parent)
	: QWidget(parent)
	, _splashLabel(nullptr)
	, ui(new Ui::BlogEditArea)
	, _toolBar(nullptr)
	, _articleManagerBtn(nullptr)
	, _messengerBtn(nullptr)
	, _linkWidgetBtn(nullptr)
	, _articleHeadView(nullptr)
	, _messageWidget(nullptr)
	, _editView(nullptr)
	, _blogHeadModel(nullptr)
	, _proxyModel(nullptr)
	, _isUserCreateArticle(false)
{
}

BlogEditArea::~BlogEditArea()
{
	delete ui;
}

bool BlogEditArea::initWidgets()
{
	_toolBar = new QFrame(this);
	_articleManagerBtn = new QToolButton(_toolBar);
    _articleManagerBtn->setCursor(Qt::PointingHandCursor);
	_messengerBtn = new QToolButton(_toolBar);
    _messengerBtn->setCursor(Qt::PointingHandCursor);
	_linkWidgetBtn = new QToolButton(_toolBar);
    _linkWidgetBtn->setCursor(Qt::PointingHandCursor);
    _messageWidget = new MessageWidget(this);

	ui->setupUi(this);
	initToolBar();
	initArticleManager();
    ui->toolWidgets->addWidget(_messageWidget);
	ui->toolWidgets->addWidget(new QFrame(this));
	_editView = new EditView(this);
	ui->editorContainLayout->addWidget(_editView);

	_articleHeadView->setDragEnabled(true);//使用QListView默认的拖拽!!!

	//必须先读取博客预览界面的框架
	initBlogFrame();

	//模型
	_blogHeadModel = new QStandardItemModel(this);
	_proxyModel = new CustomFilterModel(this);

	_proxyModel->setSourceModel(_blogHeadModel);
	_articleHeadView->setModel(_proxyModel);

	//读取用户信息
	readInfo();

	connect(_customList, &QListWidget::currentItemChanged,
			this, &BlogEditArea::selectCustom);
	connect(_customList, &CustomListWidget::blogCustomChanged,
			_proxyModel, &CustomFilterModel::setFilterCustom);

	connect(_articleHeadView, &BlogHeadView::currentIndexChanged,
			this, &BlogEditArea::setCurrentArticle);//检查当前是否选择了文档

	//初始化状态
	_articleManagerBtn->setChecked(true);//默认打开文章管理器
	_customList->setCurrentRow(0);//默认文章分类选中“全部”
	//浏览窗口QWebView第一次加载网页时需要较长时间，利用欢迎界面，在初始化的时候预加载一次
	if (_proxyModel->rowCount() > 0)
	{
		_articleHeadView->setCurrentIndex(_proxyModel->index(0, 0));
		_toolBar->hide();
		ui->toolWidgets->hide();
		ui->editorArea->hide();

		connect(_editView, &EditView::loadFinished, this, &BlogEditArea::waitForWebView);
	}
	else
	{
		waitForWebView();
	}
	return true;
}

bool BlogEditArea::customReviewer(const QString &custom)
{
	auto list = _customList->findItems(custom, Qt::MatchFixedString);
	return list.isEmpty();
}

void BlogEditArea::closeEvent(QCloseEvent *event)
{
	if (saveInfo()) QWidget::closeEvent(event);
}

void BlogEditArea::waitForWebView()
{
	Q_ASSERT (_splashLabel != nullptr);
	disconnect(_editView, &EditView::loadFinished, this, &BlogEditArea::waitForWebView);
	_splashLabel->close();
	_splashLabel = nullptr;
	_toolBar->show();
	ui->toolWidgets->show();
	ui->editorArea->show();
	ui->saveBtn->hide();
	ui->cancelBtn->hide();
    this->showMaximized();
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
			_customList->addItem(custom);
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
	int customCount = _customList->count();
	for (int i = 1; i != customCount; ++i)//从1开始（忽略“全部”）
	{
		QString custom = _customList->item(i)->data(Qt::DisplayRole).toString();
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
	_editView->show();
}

void BlogEditArea::hideEditor()
{
	QSize s = _editView->size();
	_editView->hide();
	ui->bgLabel->resize(s);
	ui->bgLabel->show();
}

void BlogEditArea::editArticle(const QString &title)
{
	Article *article = _editView->getArticle();
	article->setTitle(title);

	if (title.isEmpty())
	{
		article->setContent(QString());
	}
	else//如果是编辑已存在的文章
	{
		QFile articleFile(articleHtmlFilePath(_articleHeadView->currentIndex()));
		articleFile.open(QIODevice::ReadOnly | QIODevice::Text);
		Q_ASSERT(articleFile.isOpen());
		QString pageHtml = articleFile.readAll();//读取HTML
		articleFile.close();

		QString headerStr("<div class=\"articleContent\">");
		int beg = pageHtml.indexOf(headerStr) + headerStr.size();//内容开头
		QRegExp endRegExp("</div>");
		int end = endRegExp.indexIn(pageHtml, beg);//内容结尾
		article->setContent(pageHtml.mid(beg, end - beg));//设置内容
	}

	ui->toolWidgets->hide();
	showEditor();
	ui->editBtn->hide();
	ui->saveBtn->show();
	ui->cancelBtn->show();

	//当editor读取编辑页面时，网页中的Javascript函数（自定义，在postArticle.js）
	//会使用_editor->article来初始化网页中文章的标题和内容
	_editView->load(POST_ARTICLE_HTML_URL());
}

void BlogEditArea::saveArticle(Article *article)
{
	//停止正在阅读的页面，可能就是当前编写的页面（会崩溃？有必要停止吗？）
	_editView->stop();

	//假定正在编写的文件是当前文件！！！
	_proxyModel->setData(_articleHeadView->currentIndex(),
									   article->title(), Qt::DisplayRole);
	//保存为HTML文件，方便WebView直接Load？？？
	QFile targetHtmlFile(articleHtmlFilePath(_articleHeadView->currentIndex()));
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

	if (_customList->currentRow() != 0)//“全部”是第0个条目，不算分类
	{
		QListWidgetItem *customItem = _customList->currentItem();
		Q_ASSERT(customItem != nullptr);//必须选中一种（有可能什么都没选中吗？）
		articleItem->setData(customItem->data(Qt::DisplayRole), CustomFilterModel::CUSTOM_ROLE());
	}
	articleItem->setData(getFreeID(), CustomFilterModel::ID_ROLE());//获取独特ID
	articleItem->setData(false, CustomFilterModel::IS_PUBLISH_ROLE());//未发布
	_blogHeadModel->appendRow(articleItem);
	_proxyModel->invalidate();
	QModelIndex index = _blogHeadModel->indexFromItem(articleItem);
	index = _proxyModel->mapFromSource(index);
	_articleHeadView->setCurrentIndex(index);
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
	new QListWidgetItem(custom, _customList);
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
	QString articlePath = articleHtmlFilePath(_articleHeadView->currentIndex());
	Q_ASSERT(QFileInfo::exists(articlePath));
	_editView->load(QUrl::fromLocalFile(articlePath));
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
	_proxyModel->setFilterCustom(_customList->row(currentItem) != 0 ?
				currentItem->data(Qt::DisplayRole).toString()
			  : QString());
}

void BlogEditArea::on_editBtn_clicked()
{
	QModelIndex index = _articleHeadView->currentIndex();
	editArticle(index.data().toString());
}

void BlogEditArea::on_cancelBtn_clicked()
{
	_isUserCreateArticle = false;

	QModelIndex index = _articleHeadView->currentIndex();
	if (index.data().toString().isEmpty())
	{
		QFile::remove(articleHtmlFilePath(index));
		obsoleteID(index.data(CustomFilterModel::ID_ROLE()).toInt());
		_articleHeadView->model()->removeRow(index.row(), index.parent());
	}

//	_editor->load(QUrl::fromLocalFile(articleHtmlFilePath(ui->blogHeadView->currentIndex())));
	ui->cancelBtn->hide();
	ui->saveBtn->hide();
	ui->editBtn->show();
	ui->toolWidgets->show();
	setCurrentArticle(_articleHeadView->currentIndex());
}

void BlogEditArea::on_saveBtn_clicked()
{
	//调用网页里JavaScript的一个函数（在postArticle.js）
	//它是我自己写的，它将文章的标题、内容赋值到article.title和article.content
	_editView->page()->mainFrame()->evaluateJavaScript("updateArticle()");
	//看到输出之后就会清楚前端发送的数据是怎样的HTML格式的数据
	qDebug() << "Title " << _editView->getArticle()->title() << endl
			 << "Content " << _editView->getArticle()->content();

	if (_editView->getArticle()->title().isEmpty())
	{
		QMessageBox::warning(this, "标题不能为空", "文章标题不能为空！");
		return ;
	}

	saveArticle(_editView->getArticle());
}

void BlogEditArea::initToolBar()
{
	QVBoxLayout *layout = new QVBoxLayout(_toolBar);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(5);

	_articleManagerBtn->setIcon(QIcon(":/Image/ArticleManager.png"));
	_articleManagerBtn->setIconSize(QSize(36, 36));
	_articleManagerBtn->setCheckable(true);
	connect(_articleManagerBtn, &QToolButton::clicked, this, &BlogEditArea::selectArticleManager);
	layout->addWidget(_articleManagerBtn, 1);

	_messengerBtn->setIcon(QIcon(":/Image/Messenger.png"));
	_messengerBtn->setIconSize(QSize(36, 36));
	_messengerBtn->setCheckable(true);
	connect(_messengerBtn, &QToolButton::clicked, this, &BlogEditArea::selectMessenger);
	layout->addWidget(_messengerBtn, 1);

	_linkWidgetBtn->setIcon(QIcon(":/Image/LinkWidget.png"));
	_linkWidgetBtn->setIconSize(QSize(36, 36));
	_linkWidgetBtn->setCheckable(true);
	connect(_linkWidgetBtn, &QToolButton::clicked, this, &BlogEditArea::selectLinkWidget);
	layout->addWidget(_linkWidgetBtn, 1);

	layout->addStretch(10);

	(qobject_cast<QHBoxLayout*>(this->layout()))->insertWidget(0, _toolBar);

//	_toolBar->setStyleSheet("QFrame{background-color:white;}");
    _toolBar->setObjectName(QStringLiteral("toolBar"));
}

void BlogEditArea::initArticleManager()
{
	QFrame *articleManager = new QFrame(ui->toolWidgets);
	QHBoxLayout *managerLayout = new QHBoxLayout(articleManager);
	managerLayout->setContentsMargins(0, 0, 0, 0);
	managerLayout->setSpacing(0);
	QFont font;
    QSizePolicy horizontalfixed(QSizePolicy::Minimum, QSizePolicy::Preferred);
	//文章自定义分类管理器
	_customFrame = new QFrame(ui->toolWidgets);
	_customFrame->setObjectName(QStringLiteral("customFrame"));
	horizontalfixed.setHeightForWidth(_customFrame->sizePolicy().hasHeightForWidth());
	_customFrame->setSizePolicy(horizontalfixed);
	_customFrame->setMinimumSize(QSize(160, 0));

	//.栅格布局
	auto gridLayout = new QGridLayout(_customFrame);
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);

	auto createArticleBtn = new QPushButton(_customFrame);
    createArticleBtn->setCursor(Qt::PointingHandCursor);
    createArticleBtn->setToolTip(QString("创建新博客"));
	createArticleBtn->setObjectName(QStringLiteral("createArticleBtn"));
	createArticleBtn->setIcon(QIcon(QStringLiteral(":/Image/NewBlog.png")));
	createArticleBtn->setIconSize(QSize(18, 18));
	connect(createArticleBtn, &QPushButton::pressed, this, &BlogEditArea::userCreateArticle);

	gridLayout->addWidget(createArticleBtn, 0, 0, 1, 1);

	auto createCustomBtn = new QPushButton(_customFrame);
    createCustomBtn->setCursor(Qt::PointingHandCursor);
    createCustomBtn->setToolTip(QString("创建新分类"));
	createCustomBtn->setObjectName(QStringLiteral("createCustomBtn"));
	createCustomBtn->setIcon(QIcon(QStringLiteral(":/Image/NewCustom.png")));
	createCustomBtn->setIconSize(QSize(18, 18));
	connect(createCustomBtn, &QPushButton::pressed, this, &BlogEditArea::userCreateCustom);

	gridLayout->addWidget(createCustomBtn, 0, 1, 1, 1);

	_customList = new CustomListWidget(_customFrame);
	_customList->setObjectName(QStringLiteral("customList"));
	horizontalfixed.setHeightForWidth(_customList->sizePolicy().hasHeightForWidth());
	_customList->setSizePolicy(horizontalfixed);
	_customList->setMinimumSize(QSize(160, 0));
	font.setPointSize(12);
	_customList->setFont(font);
	_customList->setFrameShadow(QFrame::Plain);
	_customList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_customList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
	_customList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_customList->setProperty("showDropIndicator", QVariant(false));
	_customList->setDragDropMode(QAbstractItemView::DropOnly);
	_customList->setResizeMode(QListView::Adjust);
	_customList->setUniformItemSizes(true);
	_customList->setWordWrap(false);
	gridLayout->addWidget(_customList, 1, 0, 1, 2);
	new QListWidgetItem("全部", _customList);

	//.行高1比10
	gridLayout->setRowStretch(0, 1);
	gridLayout->setRowStretch(1, 10);

	managerLayout->addWidget(_customFrame);

	//文章条目管理器
	_articleHeadFrame = new QFrame(ui->toolWidgets);
	_articleHeadFrame->setObjectName(QStringLiteral("articleHeadFrame"));
	_articleHeadFrame->setMaximumSize(QSize(150, 16777215));

	//.垂直布局
	QVBoxLayout *vLayout = new QVBoxLayout(_articleHeadFrame);
	vLayout->setSpacing(0);
	vLayout->setContentsMargins(0, 0, 0, 0);

	//.
	_filterEdit = new QLineEdit(_articleHeadFrame);
	_filterEdit->setObjectName(QStringLiteral("filterEdit"));
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy2.setHeightForWidth(_filterEdit->sizePolicy().hasHeightForWidth());
	_filterEdit->setSizePolicy(sizePolicy2);
	_filterEdit->setMinimumSize(QSize(0, 24));
	_filterEdit->setMaximumSize(QSize(16777214, 24));
	_filterEdit->setFrame(true);
	_filterEdit->setClearButtonEnabled(true);
	vLayout->addWidget(_filterEdit);

	//.
	_articleHeadView = new BlogHeadView(_articleHeadFrame);
	_articleHeadView->setObjectName(QStringLiteral("articleHeadView"));
	horizontalfixed.setHeightForWidth(_articleHeadView->sizePolicy().hasHeightForWidth());
	_articleHeadView->setSizePolicy(horizontalfixed);
	font.setPointSize(13);
	_articleHeadView->setFont(font);
	_articleHeadView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_articleHeadView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
	_articleHeadView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_articleHeadView->setResizeMode(QListView::Fixed);
	_articleHeadView->setWordWrap(true);

	vLayout->addWidget(_articleHeadView);

	managerLayout->addWidget(_articleHeadFrame);

	ui->toolWidgets->addWidget(articleManager);
}

//setChecked不会发出checked信号
void BlogEditArea::selectArticleManager(bool isSelect)
{
	if (isSelect)
	{
		ui->toolWidgets->setCurrentIndex(0);//-!!!
		ui->toolWidgets->show();
		_messengerBtn->setChecked(false);
		_linkWidgetBtn->setChecked(false);
	}
	else
		ui->toolWidgets->hide();
}

void BlogEditArea::selectMessenger(bool isSelect)
{
	if (isSelect)
	{
		ui->toolWidgets->setCurrentIndex(1);//-!!!
		ui->toolWidgets->show();
		_articleManagerBtn->setChecked(false);
		_linkWidgetBtn->setChecked(false);
	}
	else
		ui->toolWidgets->hide();
}

void BlogEditArea::selectLinkWidget(bool isSelect)
{
	if (isSelect)
	{
		ui->toolWidgets->setCurrentIndex(2);//-!!!
		ui->toolWidgets->show();
		_articleManagerBtn->setChecked(false);
		_messengerBtn->setChecked(false);
	}
	else
		ui->toolWidgets->hide();
}
