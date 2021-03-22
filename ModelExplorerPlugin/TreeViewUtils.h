#pragma once

#include "TreeViewItemRole.h"

#include <QtGui/QStandardItemModel>

#include <guiddef.h>


bool tryGetIntegerData(QStandardItemModel* pItemModel, const QModelIndex& index, eTreeViewItemRole role, int& result);
bool tryGetGUID(QStandardItemModel* pItemModel, const QModelIndex& index, eTreeViewItemRole role, GUID& result);
