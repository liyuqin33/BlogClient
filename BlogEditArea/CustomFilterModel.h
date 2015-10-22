#ifndef TEXTSORTFILTERPROXYMODEL_H
#define TEXTSORTFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

class CustomFilterModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	CustomFilterModel(QObject *parent = nullptr);

	static int CUSTOM_ROLE();
	static int IS_PUBLISH_ROLE();
	static int ID_ROLE();
	static int CATEGORY_ROLE();
	static int ATTACHED_ROLE();
	static int TAG_ROLE();
public slots:
	void setFilterCustom(const QString &custom);
protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

	QString _filterCustom;
};

#endif // TEXTSORTFILTERPROXYMODEL_H
