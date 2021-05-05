#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(
      Renga::IApplicationPtr pApplication,
      ReinforcementUnitUsageAccess reinforcementUnitUsageAccess);

  PropertyList createParameters(PropertyManager& mngr) override;
  PropertyList createQuantities(PropertyManager& mngr) override;
  PropertyList createProperties(PropertyManager& mng) override { return{}; };

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  ReinforcementUnitUsageAccess m_reinforcementUnitUsageAccess;
};
