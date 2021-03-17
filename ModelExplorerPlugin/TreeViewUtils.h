#pragma once

#include "TreeViewItemRole.h"

#include <QtGui/QStandardItemModel>


bool tryGetIntegerData(QStandardItemModel* pItemModel, const QModelIndex& index, eTreeViewItemRole role, int& result);
