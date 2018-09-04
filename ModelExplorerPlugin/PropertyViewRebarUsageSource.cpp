#include "stdafx.h"

#include "PropertyViewRebarUsageSource.h"
#include "RebarUsagePropertyViewBuilder.h"

PropertyViewRebarUsageSource::PropertyViewRebarUsageSource(
  Renga::IApplicationPtr pApplication,
  Renga::IRebarUsagePtr pRebarUsage) :
  m_pApplication(pApplication),
  m_pRebarUsage(pRebarUsage)
{
}

IPropertyViewBuilder* PropertyViewRebarUsageSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  return new RebarUsagePropertyViewBuilder(pPropertyManagers, m_pApplication, m_pRebarUsage);
}

Renga::IPropertyPtr PropertyViewRebarUsageSource::getUserDefinedProperty(GUID propertyId)
{
  return nullptr;
}