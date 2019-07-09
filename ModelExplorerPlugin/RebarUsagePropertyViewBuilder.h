#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class RebarUsagePropertyViewBuilder : public CPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                Renga::IApplicationPtr pApplication,
                                Renga::IRebarUsagePtr pRebarUsage);

  void createIntegratedParameters(PropertyList& propertyList) override;
  void createQuantities(PropertyList& propertyList) override;

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IRebarUsagePtr m_pRebarUsage;
};
