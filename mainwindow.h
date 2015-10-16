#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QStackedWidget>
#include <QFile>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

#include "titlebar.h"
#include "BlogEditArea.h"
#include "MyUtility.h"

#define MARGIN 5
class MainWindow : public QFrame
{
    Q_OBJECT

public:
    /*各种方向*/
    enum Direction {NONE, TOP, RIGHT, BOTTOM, LEFT, TOP_RIGHT, RIGHT_BOTTOM, BOTTOM_LEFT, LEFT_TOP};
	MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void initUi();
private slots:
	void waitForWebView();

private:
	void initQss();
    void initLayout();
    void initConnect();

    Direction _direction;           //方向
    bool _isLeftPressed;            //左键
    QPoint _dragPosition;           //左键按下的位置
    TitleBar *_titleBar;            //菜单栏

    QStackedWidget *_functionWidget;
    BlogEditArea *_blogEditArea;
    QVBoxLayout *_mainLayout;

	//欢迎Label
	QLabel *_splashLabel;

    Direction judgeDirection(int x, int y);      //判断鼠标方向
    void setCursorStyle(Direction dir);         //改变光标样式
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
