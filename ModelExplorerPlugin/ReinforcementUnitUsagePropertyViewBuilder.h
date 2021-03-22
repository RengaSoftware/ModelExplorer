#pragma once

#include "IPropertyViewBuilder.h"


class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                            Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void createParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
