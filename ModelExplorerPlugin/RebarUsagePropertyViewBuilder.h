#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                RebarUsageAccess rebarUsageAccess);

  PropertyList createParameters(PropertyManager& mngr) override;
  PropertyList createQuantities(PropertyManager& mngr) override;

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  RebarUsageAccess m_rebarUsageAccess;
};
