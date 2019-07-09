#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class ReinforcementUnitUsagePropertyViewBuilder : public CPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                            Renga::IApplicationPtr pApplication,
                                            Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void createIntegratedParameters(PropertyList& propertyList) override;
  void createQuantities(PropertyList& propertyList) override;

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  const PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
