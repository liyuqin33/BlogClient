#include "article.h"
#include <QDebug>

Article::Article(QObject *parent) :
	QObject(parent)
{
}

QString Article::title() const
{
	return _title;
}

void Article::setTitle(const QString &title)
{
	_title = title;
}
QString Article::content() const
{
	return _content;
}

void Article::setContent(const QString &content)
{
	_content = content;
}


