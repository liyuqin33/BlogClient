#include "MyUtility.h"
#include <QDir>
#include "BlogEditArea/CustomFilterModel.h"
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
<<<<<<< HEAD
	static const QString blogInfoJsonPath = QDir::currentPath() + "/BlogEdit.json";
=======
    static const QString blogInfoJsonPath = QDir::currentPath() + "/BlogEdit.json";
>>>>>>> refs/heads/pr/9
    return blogInfoJsonPath;
}

const QString &USER_INFO_JSON_PATH()
{
	static const QString userInfoJsonPath = QDir::currentPath() + "/UserData.json";
    return userInfoJsonPath;
}
