#ifndef PICLABEL_H
#define PICLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class PicLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PicLabel(QWidget *parent = 0);
    ~PicLabel();
protected:
    void mouseReleaseEvent(QMouseEvent * ev);
signals:
    void clicked();

public slots:
};

#endif // PICLABEL_H
