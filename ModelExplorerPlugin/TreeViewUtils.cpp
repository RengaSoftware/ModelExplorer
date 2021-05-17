#include "stdafx.h"

#include "TreeViewUtils.h"
#include "COMUtils.h"


bool tryGetIntegerData(
  QStandardItemModel *pItemModel,
  const QModelIndex &index,
  eTreeViewItemRole role, int &result)
{
  QVariant data = pItemModel->data(index, role);
  if (!data.isValid())
    return false;

  bool ok = false;
  int value = data.toInt(&ok);

  if (ok)
    result = value;

  return ok;
}

bool tryGetGUID(QStandardItemModel * pItemModel, const QModelIndex & index, eTreeViewItemRole role, GUID & result)
{
  QVariant data = pItemModel->data(index, role);
  if (!data.isValid())
    return false;

  auto value = data.toString();
  result = GuidFromString(value.toStdString());
  return true;
}