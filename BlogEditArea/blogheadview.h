#ifndef BLOGHEADVIEW_H
#define BLOGHEADVIEW_H
#include <QListView>


class BlogHeadView : public QListView
{
	Q_OBJECT
public:
	explicit BlogHeadView(QWidget *parent = nullptr);
signals:
	void currentIndexChanged(QModelIndex index);
protected:
	void currentChanged(const QModelIndex & current, const QModelIndex & previous)override;
};

#endif // BLOGHEADVIEW_H
