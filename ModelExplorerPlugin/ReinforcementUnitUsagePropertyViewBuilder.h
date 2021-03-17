#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                            Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
