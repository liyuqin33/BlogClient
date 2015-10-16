#include "BlogEditArea.h"
#include <QApplication>
#include <QDateTime>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	BlogEditArea w;
	w._splashLabel = new QLabel(&w);
	w._splashLabel->setPixmap(QPixmap(":/BlogImages/hello.jpg"));
	w._splashLabel->setScaledContents(true);
	w._splashLabel->setAttribute(Qt::WA_DeleteOnClose);
	//UI会resize BlogEditArea窗口，所以showMaximized没用
//	w._splashLabel->resize(QApplication::desktop()->availableGeometry().size() - (w.frameSize() - w.size()));
	w._splashLabel->resize(1214, 577);
	w._splashLabel->move(0, 0);
	qDebug() << QApplication::desktop()->availableGeometry().size() - (w.frameSize() - w.size());
	w.move(100, 0);
	w.show();
	a.processEvents();//处理paintEvent
	w.initWidgets();
    return a.exec();
}
