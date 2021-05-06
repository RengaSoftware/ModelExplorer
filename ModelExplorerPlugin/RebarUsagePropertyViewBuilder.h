#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class RebarUsagePropertyViewBuilder : public IPropertyViewBuilder
{
public:
  RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                RebarUsageAccess rebarUsageAccess);

  void createParameters(PropertyManager& mngr) override;
  void createQuantities(PropertyManager& mngr) override;
  void createProperties(PropertyManager& mng) override {}

private:
  Renga::IRebarStylePtr getRebarStyle(int styleId) const;

private:
  Renga::IApplicationPtr m_pApplication;
  RebarUsageAccess m_rebarUsageAccess;
};
