#pragma once

#include "IObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

class RebarUsagePropertyViewBuilder : public IObjectPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(
    const PropertyManagers* pPropertyManagers,
    Renga::IApplicationPtr pApplication,
    Renga::IRebarUsagePtr pRebarUsage);

  void createParametersProperties(PropertyList& propertyList) override;
  void createQuantitiesProperties(PropertyList& propertyList) override;
  PropertyList createUserAttributesProperties() override;

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  const PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IRebarUsagePtr m_pRebarUsage;
};
