#pragma once

#include "IPropertyViewSourceObject.h"

class PropertyViewRebarUsageSource : public CPropertyViewSourceObject
{
public:
  PropertyViewRebarUsageSource(Renga::IApplicationPtr pApplication,
                               Renga::IRebarUsagePtr pRebarUsage);

  std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder(PropertyManagers* pPropertyManagers) override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IRebarUsagePtr m_pRebarUsage;
};
