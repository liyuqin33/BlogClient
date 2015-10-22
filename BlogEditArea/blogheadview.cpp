#include "blogheadview.h"

BlogHeadView::BlogHeadView(QWidget *parent) :
	QListView(parent)
{
}

void BlogHeadView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	QListView::currentChanged(current, previous);
	emit currentIndexChanged(current);
}

