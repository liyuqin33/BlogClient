#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>

#include "statewidget.h"

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QWidget *parent = 0);
    ~MessageWidget();

private:
    void initUi();
    void initLayout();

    QTabWidget *_tabWidget;             //主框体
    StateWidget *_firstWidget;          //社区动态
    StateWidget *_secondWidget;         //系统通知

};

#endif // MESSAGEWIDGET_H
