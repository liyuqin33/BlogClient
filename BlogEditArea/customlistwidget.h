#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H
#include <QListWidget>

class CustomListWidget : public QListWidget
{
	Q_OBJECT
public:
	CustomListWidget(QWidget *parent = nullptr);
signals:
	void blogCustomChanged(QString custom);

protected:
	void dropEvent(QDropEvent *event)override;

};

#endif // CUSTOMLISTWIDGET_H
