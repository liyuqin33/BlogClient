#include "mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *hello = new QSplashScreen(QPixmap(":/BlogImages/hello.jpg"));
    hello->show();

    a.processEvents();

    QDateTime n = QDateTime::currentDateTime();
    QDateTime now;
    do
    {
       now = QDateTime::currentDateTime();
    } while (n.secsTo(now) <= 1);

    MainWindow w;
    w.show();





    hello->finish(&w);
    return a.exec();
}
