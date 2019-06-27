#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class ReinforcementUnitUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  ReinforcementUnitUsagePropertyViewBuilder(
    const PropertyManagers* pPropertyManagers,
    Renga::IApplicationPtr pApplication,
    Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void createParametersProperties(PropertyList& propertyList) override;
  void createParametersPropertiesEx(PropertyList& propertyList) override;
  void createQuantitiesProperties(PropertyList& propertyList) override;
  PropertyList createUserAttributesProperties() override;

private:
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int styleId) const;

private:
  const PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
