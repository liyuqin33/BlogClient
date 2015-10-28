#include "piclabel.h"

PicLabel::PicLabel(QWidget *parent) : QLabel(parent)
{

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

