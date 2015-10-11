#include "mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	w.show();//MainWindow现在只有label并且只显示label
	a.processEvents();//处理paintEvent
	w.initUi();//初始化主要部件
    return a.exec();
}
