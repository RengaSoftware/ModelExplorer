#pragma once

#include <QtCore/qnamespace.h>


enum eTreeViewItemRole
{
  ItemType = Qt::UserRole + 1,
  EntityId,
  EntityType,
  LayerIndex,
  IsVisible,
  RebarUsageIndex,
  ReinforcementUnitUsageIndex,
  ReinforcementUnitStyleId
};
