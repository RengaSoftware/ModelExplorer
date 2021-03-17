#include "stdafx.h"

#include "PropertyViewRebarUsageSource.h"
#include "RebarUsagePropertyViewBuilder.h"

PropertyViewRebarUsageSource::PropertyViewRebarUsageSource(Renga::IApplicationPtr pApplication,
                                                           Renga::IRebarUsagePtr pRebarUsage) :
  m_pApplication(pApplication),
  m_pRebarUsage(pRebarUsage)
{
}

std::unique_ptr<IPropertyViewBuilder> PropertyViewRebarUsageSource::createPropertyViewBuilder()
{
  return std::make_unique<RebarUsagePropertyViewBuilder>(m_pApplication, m_pRebarUsage);
}
