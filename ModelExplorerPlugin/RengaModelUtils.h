#pragma once

#include <guiddef.h>


Renga::IEntityCollectionPtr getProjectEntityCollection(Renga::IProjectPtr pProject, GUID entityType);

using PropertyContainerAccess = std::function<Renga::IPropertyContainerPtr()>;
using ParameterContainerAccess = std::function<Renga::IParameterContainerPtr()>;
using QuantityContainerAccess = std::function<Renga::IQuantityContainerPtr()>;