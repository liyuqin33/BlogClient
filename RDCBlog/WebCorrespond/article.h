#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>
#include <QStringList>
/* class Article
 * 前端是直接把tags当成一个字符串发给后台的吗？
*/

class Article : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(QString content READ content WRITE setContent)
public:
	explicit Article(QObject *parent = 0);

	QString title() const;
	void setTitle(const QString &title);

	QString content() const;
	void setContent(const QString &content);

private:
	QString _title;
	QString _content;
};

#endif // ARTICLE_H
