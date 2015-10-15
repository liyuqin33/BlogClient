#include "splashwindow.h"
#include <QHBoxLayout>

SplashWindow::SplashWindow()
	: QWidget(nullptr)
	, _splashLabel(new QLabel(this))
	, _showWindow(nullptr)
{
	_splashLabel->setPixmap(QPixmap(":/BlogImages/hello.jpg"));
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(_splashLabel);
	setLayout(layout);
	this->setAttribute(Qt::WA_DeleteOnClose);
}

void SplashWindow::setShowWindow(QWidget *showWindow)
{
	_showWindow = showWindow;
}

void SplashWindow::finish()
{
	Q_ASSERT(_showWindow != nullptr);
	_showWindow->show();
	this->close();
}

