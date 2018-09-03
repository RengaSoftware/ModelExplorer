#include "stdafx.h"

#include "PropertyViewMaterialLayerSource.h"
#include "MaterialLayerPropertyViewBuilder.h"

PropertyViewMaterialLayerSource::PropertyViewMaterialLayerSource(
  Renga::IApplicationPtr pApplication,
  Renga::IMaterialLayerPtr pMaterialLayer,
  Renga::ILayerPtr pLayer) :
  m_pApplication(pApplication),
  m_pMaterialLayer(pMaterialLayer),
  m_pLayer(pLayer)
{
}

IObjectPropertyViewBuilder* PropertyViewMaterialLayerSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  return new MaterialLayerPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pMaterialLayer, m_pLayer);
}

Renga::IPropertyPtr PropertyViewMaterialLayerSource::getUserDefinedProperty(GUID propertyId)
{
  return nullptr;
}
