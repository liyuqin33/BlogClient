#include "MyUtility.h"
#include <QDir>
#include "CustomFilterModel.h"
#include <QDebug>

const QString& STORE_PATH()
{
	static const QString storePath = QDir::currentPath() + "/RDCBlog/store";
	return storePath;
}

const QUrl &POST_ARTICLE_HTML_URL()
{
	static const QUrl postArticleHtmlUrl = QUrl::fromLocalFile(QDir::currentPath() + "/RDCBlog/html/postArticle.html");
	return postArticleHtmlUrl;
}


const QString &BLOG_INFO_JSON_PATH()
{
	static const QString blogInfoJsonPath = STORE_PATH() + "/BlogEdit.json";
	return blogInfoJsonPath;
}
