#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(
      Renga::IApplicationPtr pApplication,
      ReinforcementUnitUsageAccess reinforcementUnitUsageAccess);

  void createParameters(PropertyManager& mngr) const override;
  void createQuantities(PropertyManager& mngr) const override;
  void createProperties(PropertyManager& mng) const override{};

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  ReinforcementUnitUsageAccess m_reinforcementUnitUsageAccess;
};
