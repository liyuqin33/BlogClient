#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QWidget>
#include <QLabel>

class SplashWindow : public QWidget
{
	Q_OBJECT
public:
	explicit SplashWindow();

	void setShowWindow(QWidget *showWindow);
public slots:
	void finish();

private:
	QLabel *_splashLabel;
	QWidget *_showWindow;
};

#endif // SPLASHWINDOW_H
