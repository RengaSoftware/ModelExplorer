#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(
      Renga::IApplicationPtr pApplication,
      ReinforcementUnitUsageAccess reinforcementUnitUsageAccess);

  void createParameters(PropertyManager& mngr) override;
  void createQuantities(PropertyManager& mngr) override;
  void createProperties(PropertyManager& mng) override {};

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  ReinforcementUnitUsageAccess m_reinforcementUnitUsageAccess;
};
