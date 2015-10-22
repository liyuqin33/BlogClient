#ifndef EDITVIEW_H
#define EDITVIEW_H
#include <QWebView>
#include "WebCorrespond/article.h"


class EditView : public QWebView
{
	Q_OBJECT
public:
	explicit EditView(QWidget *parent);
	Article* getArticle(){return _article;}
private slots:
	void addContentElementToClient();
protected:
	Article *_article;
};

#endif // EDITVIEW_H
