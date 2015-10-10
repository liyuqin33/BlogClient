#include "customlistwidget.h"
#include <QDropEvent>
#include <QListView>
#include "CustomFilterModel.h"

CustomListWidget::CustomListWidget(QWidget *parent) :
	QListWidget(parent)
{
}

void CustomListWidget::dropEvent(QDropEvent *event)
{
	auto customItem = this->itemAt(event->pos());
	if (customItem == nullptr) return;
	QListView *blogHeadView = dynamic_cast<QListView*>(event->source());
	Q_ASSERT_X(blogHeadView, "In CategoryWidget::dropEvent", "Can't get the BlogHeadView!");
	QItemSelectionModel *sel= blogHeadView->selectionModel();
	Q_ASSERT_X(sel->hasSelection(), "In CategoryWidget::dropEvent", "BlogHeadView must has selection when draging!");

	QString custom = this->row(customItem) == 0 ? "" : customItem->data(Qt::DisplayRole).toString();
	QModelIndexList indexList = sel->selectedRows();
	foreach (QModelIndex index, indexList)
	{
		blogHeadView->model()->setData(index, custom, CustomFilterModel::CUSTOM_ROLE());
	}
	this->setCurrentItem(customItem);

	event->accept();
	emit blogCustomChanged(custom);
}

