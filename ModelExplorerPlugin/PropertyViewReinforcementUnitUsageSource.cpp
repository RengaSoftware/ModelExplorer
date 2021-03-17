#include "stdafx.h"

#include "PropertyViewReinforcementUnitUsageSource.h"
#include "ReinforcementUnitUsagePropertyViewBuilder.h"

PropertyViewReinforcementUnitUsageSource::PropertyViewReinforcementUnitUsageSource(Renga::IApplicationPtr pApplication,
                                                                                   Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage) :
  m_pApplication(pApplication),
  m_pReinforcementUnitUsage(pReinforcementUnitUsage)
{
}

std::unique_ptr<IPropertyViewBuilder> PropertyViewReinforcementUnitUsageSource::createPropertyViewBuilder()
{
  return std::make_unique<ReinforcementUnitUsagePropertyViewBuilder>(m_pApplication, m_pReinforcementUnitUsage);
}
