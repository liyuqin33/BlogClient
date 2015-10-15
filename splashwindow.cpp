#include "splashwindow.h"
#include <QHBoxLayout>
#include <QDebug>

SplashWindow::SplashWindow()
	: QWidget(nullptr)
	, _splashLabel(new QLabel(this))
	, _showWindow(nullptr)
{
	_splashLabel->setPixmap(QPixmap(":/BlogImages/hello.jpg"));
	_splashLabel->setScaledContents(true);
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_splashLabel);
	this->setAttribute(Qt::WA_DeleteOnClose);
}

void SplashWindow::setShowWindow(QWidget *showWindow)
{
	_showWindow = showWindow;
}

void SplashWindow::finish()
{
	Q_ASSERT(_showWindow != nullptr);
	_showWindow->showMaximized();
	this->close();
}

