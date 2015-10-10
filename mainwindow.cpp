#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _direction = NONE;
    _isLeftPressed = false;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->setMinimumSize(900,600);

    initQss();
    initUi();
    initLayout();

    initConnect();

}

MainWindow::~MainWindow()
{

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

    _functionWidget->addWidget(_blogEditArea);


}

void MainWindow::initLayout()
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->addWidget(_titleBar);
    _mainLayout->addWidget(_functionWidget);
    _mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(_mainLayout);
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
