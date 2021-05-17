#pragma once

#include <guiddef.h>


Renga::IEntityCollectionPtr getProjectEntityCollection(Renga::IProjectPtr pProject, GUID entityType);
QStringList getLogicalValueStringList();
// returns index in value array
int getLogicalValueIndex(Renga::Logical value);
Renga::Logical getLogicalValueFromIndex(int index);

using PropertyContainerAccess = std::function<Renga::IPropertyContainerPtr()>;
using ParameterContainerAccess = std::function<Renga::IParameterContainerPtr()>;
using QuantityContainerAccess = std::function<Renga::IQuantityContainerPtr()>;
using MaterialLayerAccess = std::function<Renga::IMaterialLayerPtr()>;
using LayerAccess = std::function<Renga::ILayerPtr()>;
using RebarUsageAccess = std::function<Renga::IRebarUsagePtr()>;
using ReinforcementUnitUsageAccess = std::function<Renga::IReinforcementUnitUsagePtr()>;