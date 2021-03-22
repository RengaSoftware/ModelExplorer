#pragma once

#include <QtCore/qnamespace.h>


enum eTreeViewItemRole
{
  ItemType = Qt::UserRole + 1,
  EntityId,
  LayerIndex,
  IsVisible,
  RebarUsageIndex,
  ReinforcementUnitUsageIndex,
  ReinforcementUnitStyleId
};
