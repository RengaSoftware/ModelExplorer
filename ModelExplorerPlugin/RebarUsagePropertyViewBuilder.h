#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                Renga::IRebarUsagePtr pRebarUsage);

  void createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IRebarUsagePtr m_pRebarUsage;
};
