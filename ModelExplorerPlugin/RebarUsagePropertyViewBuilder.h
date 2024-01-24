#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                RebarUsageAccess rebarUsageAccess);

  void createParameters(PropertyManager& mngr) const override;
  void createQuantities(PropertyManager& mngr) const override;
  void createProperties(PropertyManager& mng) const override {}

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  RebarUsageAccess m_rebarUsageAccess;
};
