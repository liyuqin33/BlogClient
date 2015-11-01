#include "Login/login.h"
#include "Login/tray.h"
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/default.qss");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    a.setStyleSheet(file.readAll());

	Login *loginWiget = new Login();
	loginWiget->setAttribute(Qt::WA_DeleteOnClose);
	//加载系统托盘
	auto tray = new Tray();
	tray->setLoginWidget(loginWiget);
	loginWiget->show();
	return a.exec();
}
