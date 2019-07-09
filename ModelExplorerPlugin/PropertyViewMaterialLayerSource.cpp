#include "stdafx.h"

#include "PropertyViewMaterialLayerSource.h"
#include "MaterialLayerPropertyViewBuilder.h"

PropertyViewMaterialLayerSource::PropertyViewMaterialLayerSource(Renga::IApplicationPtr pApplication,
                                                                 Renga::IMaterialLayerPtr pMaterialLayer,
                                                                 Renga::ILayerPtr pLayer) :
  m_pApplication(pApplication),
  m_pMaterialLayer(pMaterialLayer),
  m_pLayer(pLayer)
{
}

std::unique_ptr<IPropertyViewBuilder> PropertyViewMaterialLayerSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  return std::make_unique<MaterialLayerPropertyViewBuilder>(pPropertyManagers, m_pApplication, m_pMaterialLayer, m_pLayer);
}
