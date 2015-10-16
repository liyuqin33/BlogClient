#include "mainwindow.h"

/*
在构造函数里只初始化好必须的部件（layout，splashLabel）
其他主要的部件由main函数调用初始化，使欢迎界面掩盖初始化速度慢的事实
*/
MainWindow::MainWindow(QWidget *parent)
	: QFrame(parent)
	//尽可能使用初始值列表分配有意义的值！！！
	, _direction(NONE)
	, _isLeftPressed(false)
	, _titleBar(nullptr)
	, _functionWidget(nullptr)
	, _blogEditArea(nullptr)
	, _mainLayout(new QVBoxLayout(this))
	, _splashLabel(new QLabel())
{
    _direction = NONE;
    _isLeftPressed = false;
	//Qt::CustomizeWindowHint能使标题栏消失，并且可以拉伸窗口，但是原本是标题栏的地方还有一条白条
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setFrameStyle(QFrame::NoFrame);
    this->setMouseTracking(true);
    this->setMinimumSize(900,600);

	_mainLayout->setContentsMargins(0,0,0,0);
	_splashLabel->setPixmap(QPixmap(":/BlogImages/hello.jpg"));
	_mainLayout->addWidget(_splashLabel);
	this->setLayout(_mainLayout);
}

MainWindow::~MainWindow()
{

}

void MainWindow::waitForWebView()
{
	//断开连接
	disconnect(_blogEditArea->editView(), &QWebView::loadFinished, this, &MainWindow::waitForWebView);
	//显示
	_mainLayout->removeWidget(_splashLabel);
	_splashLabel->setAttribute(Qt::WA_DeleteOnClose, true);
	_splashLabel->close();
	_splashLabel = nullptr;
	initLayout();
	initConnect();
}

void MainWindow::initUi()
{
    _titleBar = new TitleBar(this);
    _titleBar->setObjectName("titleBar");

    _functionWidget = new QStackedWidget(this);
    /*然后在这里加入你们的功能widget*/
    //例如_functionWidget->addWidget(_testWidget);

    BlogEditArea::initBlogFrame();
    _blogEditArea = new BlogEditArea;
	connect(_blogEditArea->editView(), &QWebView::loadFinished, this, &MainWindow::waitForWebView);

    _functionWidget->addWidget(_blogEditArea);

	initQss();
}

void MainWindow::initLayout()
{
	_mainLayout->addWidget(_titleBar);
	_mainLayout->addWidget(_functionWidget);
}

void MainWindow::initConnect()
{
    connect(_titleBar,SIGNAL(clickMinBtn()),this,SLOT(showMinimized()));
    connect(_titleBar,SIGNAL(clickMaxBtn()),this,SLOT(showMaximized()));
    connect(_titleBar,SIGNAL(clickNormalBtn()),this,SLOT(showNormal()));
    connect(_titleBar,SIGNAL(clickCloseBtn()),this,SLOT(close()));

    /*在这里加入你们的功能widget的信号与槽*/
    connect(_titleBar, SIGNAL(clickFunctionBtn(int)), _functionWidget, SLOT(setCurrentIndex(int)));

}

void MainWindow::initQss()
{
    QFile qss(":/default.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
}

/*没做好*/
MainWindow::Direction MainWindow::judgeDirection(int x, int y)
{
    QPoint topL = this->mapToGlobal(this->rect().topLeft());
    QPoint bottomR = this->mapToGlobal(this->rect().bottomRight());
    if(x >= topL.x() && x <= topL.x() + MARGIN && y >= topL.y() && y <= topL.y() + MARGIN)
        return LEFT_TOP;
    if(x >= bottomR.x() - MARGIN && x <= bottomR.x() && y >= topL.y() && y <= topL.y() + MARGIN)
        return TOP_RIGHT;
    if(x >= bottomR.x() - MARGIN && x <= bottomR.x() && y >= bottomR.y() - MARGIN && y <= bottomR.y())
        return RIGHT_BOTTOM;
    if(x >= topL.x() && x <= topL.x() + MARGIN && y >= bottomR.y() - MARGIN && y <= bottomR.y())
        return BOTTOM_LEFT;
    return NONE;

}
/*没做好*/
void MainWindow::setCursorStyle(MainWindow::Direction dir)
{
    switch (dir)
    {
        case TOP:
        case BOTTOM:
             this->setCursor(Qt::SizeVerCursor);
             break;
        case RIGHT:
        case LEFT:
             this->setCursor(Qt::SizeHorCursor);
             break;
        case TOP_RIGHT:
        case BOTTOM_LEFT:
             this->setCursor(Qt::SizeBDiagCursor);
             break;
        case LEFT_TOP:
        case RIGHT_BOTTOM:
             this->setCursor(Qt::SizeFDiagCursor);
             break;

        default:
            this->setCursor(Qt::ArrowCursor);
            break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(event->y() < _titleBar->height() && event->x() <_titleBar->width())
        {
            _isLeftPressed = true;
            _dragPosition = event->globalPos();
        }
    }
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMaximized())
    {
        event->ignore();
    }
    else
    {
        /*没做好*/
        if(!_isLeftPressed)
        {
            _direction = judgeDirection(event->globalX(),event->globalY());
            setCursorStyle(_direction);
        }
        if(event->y() < _titleBar->height() && event->x() <_titleBar->width())
        {
            if(_isLeftPressed)
            {
                if(!_titleBar->getLockFlag())
                    this->move(this->pos() + (event->globalPos() - _dragPosition));
                _dragPosition = event->globalPos();
                event->accept();
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    _isLeftPressed = false;
    event->accept();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->y() < _titleBar->height() && event->x() <_titleBar->width())
    {
        _titleBar->swithMaxNormalBtn();
        event->accept();
    }else{
        event->ignore();
    }
}
