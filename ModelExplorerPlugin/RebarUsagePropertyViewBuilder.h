#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                RebarUsageAccess rebarUsageAccess);

  void createParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  RebarUsageAccess m_rebarUsageAccess;
};
