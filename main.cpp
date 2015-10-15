﻿#include "splashwindow.h"
#include "BlogEditArea.h"
#include <QApplication>
#include <QDateTime>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	SplashWindow *splashWindow = new SplashWindow();
	splashWindow->showMaximized();
	a.processEvents();//处理paintEvent
	BlogEditArea w;
	splashWindow->setShowWindow(&w);
	QObject::connect(w.editView(), &EditView::loadFinished, splashWindow, &SplashWindow::finish);
    return a.exec();
}
