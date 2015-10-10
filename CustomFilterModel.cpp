#include "CustomFilterModel.h"
#include <QDebug>


CustomFilterModel::CustomFilterModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{
}

int CustomFilterModel::CUSTOM_ROLE()
{
	static const int customRole = Qt::UserRole;
	return customRole;
}

int CustomFilterModel::IS_PUBLISH_ROLE()
{
	static const int isPublishRole = CUSTOM_ROLE() + 1;
	return isPublishRole;
}

int CustomFilterModel::ID_ROLE()
{
	static const int idRole = IS_PUBLISH_ROLE() + 1;
	return idRole;
}

int CustomFilterModel::CATEGORY_ROLE()
{
	static const int categoryRole = ID_ROLE() + 1;
	return categoryRole;
}

int CustomFilterModel::ATTACHED_ROLE()
{
	static const int attachedRole = CATEGORY_ROLE() + 1;
	return attachedRole;
}

int CustomFilterModel::TAG_ROLE()
{
	static const int tagRole = ATTACHED_ROLE() + 1;
	return tagRole;
}

void CustomFilterModel::setFilterCustom(const QString &custom)
{
	_filterCustom = custom;
	this->invalidateFilter();
}

bool CustomFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	const QString custom = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), CUSTOM_ROLE()).toString();

	return _filterCustom.isEmpty() || custom == _filterCustom;
}

