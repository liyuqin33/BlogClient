#ifndef NOFOCUSFRAMEDELEGATE_H
#define NOFOCUSFRAMEDELEGATE_H
#include <QObject>
#include <QStyledItemDelegate>


class NoFocusFrameDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	NoFocusFrameDelegate(QObject *parent = 0);
private:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // NOFOCUSFRAMEDELEGATE_H
