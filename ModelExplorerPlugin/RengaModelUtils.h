#pragma once

#include <guiddef.h>

// INFO: works only for entities located directly in project (styles, materials, drawings, etc.)
GUID parameterIdToEntityType(GUID parameterId);
Renga::IEntityCollectionPtr getProjectEntityCollection(Renga::IProjectPtr pProject, GUID entityType);
QStringList getLogicalValueStringList();
// returns index in value array
int getLogicalValueIndex(Renga::Logical value);
Renga::Logical getLogicalValueFromIndex(int index);
// INFO: works only for entities located directly in project (styles, materials, drawings, etc.)
QString getEntityName(Renga::IProjectPtr pProject, GUID entityType, int entityId);

using PropertyContainerAccess = std::function<Renga::IPropertyContainerPtr()>;
using ParameterContainerAccess = std::function<Renga::IParameterContainerPtr()>;
using QuantityContainerAccess = std::function<Renga::IQuantityContainerPtr()>;
using MaterialLayerAccess = std::function<Renga::IMaterialLayerPtr()>;
using LayerAccess = std::function<Renga::ILayerPtr()>;
using RebarUsageAccess = std::function<Renga::IRebarUsagePtr()>;
using ReinforcementUnitUsageAccess = std::function<Renga::IReinforcementUnitUsagePtr()>;
using EntityByParameterIdNameGetter = std::function<QString(GUID, int)>;
