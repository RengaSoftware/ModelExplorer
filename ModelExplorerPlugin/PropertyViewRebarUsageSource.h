#pragma once

#include "IPropertyViewSourceObject.h"

class PropertyViewRebarUsageSource : public IPropertyViewSourceObject
{
public:
  PropertyViewRebarUsageSource(Renga::IApplicationPtr pApplication,
                               Renga::IRebarUsagePtr pRebarUsage);

  std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder(PropertyManagers* pPropertyManagers) override;
  Renga::IParameterPtr getParameter(GUID parameterId) override { return nullptr; }

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IRebarUsagePtr m_pRebarUsage;
};
