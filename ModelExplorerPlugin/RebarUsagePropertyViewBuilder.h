#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
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
