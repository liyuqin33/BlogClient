#include "editview.h"
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>
#include <QCloseEvent>
#include <functional>


EditView::EditView(QWidget *parent) :
	QWebView(parent),
	_article(new Article(this))
{
	QWebSettings *setting = this->settings();
	setting->setAttribute(QWebSettings::AutoLoadImages, true);
	setting->setAttribute(QWebSettings::JavascriptEnabled, true);
	setting->setAttribute(QWebSettings::PluginsEnabled, true);
	setting->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);

	connect(this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(addContentElementToClient()));
}

void EditView::addContentElementToClient()
{
	Q_ASSERT(_article != nullptr);
	this->page()->mainFrame()->addToJavaScriptWindowObject("article", _article);
}
