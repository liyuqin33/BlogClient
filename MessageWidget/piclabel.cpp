#include "piclabel.h"

PicLabel::PicLabel(QWidget *parent) : QLabel(parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

PicLabel::~PicLabel()
{

}

void PicLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
         Q_UNUSED(ev)
         emit clicked();
    }
}

