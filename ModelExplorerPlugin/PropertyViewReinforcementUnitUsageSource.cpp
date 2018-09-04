#include "stdafx.h"

#include "PropertyViewReinforcementUnitUsageSource.h"
#include "ReinforcementUnitUsagePropertyViewBuilder.h"

PropertyViewReinforcementUnitUsageSource::PropertyViewReinforcementUnitUsageSource(
  Renga::IApplicationPtr pApplication,
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage) :
  m_pApplication(pApplication),
  m_pReinforcementUnitUsage(pReinforcementUnitUsage)
{
}

IPropertyViewBuilder* PropertyViewReinforcementUnitUsageSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  return new ReinforcementUnitUsagePropertyViewBuilder(pPropertyManagers, m_pApplication, m_pReinforcementUnitUsage);
}

Renga::IPropertyPtr PropertyViewReinforcementUnitUsageSource::getUserDefinedProperty(GUID propertyId)
{
  return nullptr;
}
