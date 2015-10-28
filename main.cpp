#include "BlogEditArea/BlogEditArea.h"
#include "Login/login.h"
#include "Login/tray.h"
#include <QApplication>
#include <QDateTime>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	BlogEditArea w;

    QFile file(":/default.qss");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    a.setStyleSheet(file.readAll());

//    w.setWindowFlags(Qt::FramelessWindowHint);
    Login qdialog(&w, true);
    qdialog.setWindowTitle("登陆界面");
    Tray *tray=qdialog._tray;
    if ( qdialog.exec() == QDialog::Accepted )
    {
        w._splashLabel = new QLabel(&w);
        w._splashLabel->setPixmap(QPixmap(":/Image/hello.jpg"));
        w._splashLabel->setScaledContents(true);
        w._splashLabel->setAttribute(Qt::WA_DeleteOnClose);
        w._splashLabel->resize(1214, 577);
        w._splashLabel->move(0, 0);
        w.move(100, 50);
        w.show();
        a.processEvents();//处理paintEvent
        w.initWidgets();

        return a.exec();
    }
    return 0;


}
