#include "stdafx.h"

#include "MaterialLayerPropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


MaterialLayerPropertyViewBuilder::MaterialLayerPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                                                   Renga::IMaterialLayerPtr pMaterialLayer,
                                                                   Renga::ILayerPtr pLayer) :
  m_pApplication(pApplication),
  m_pMaterialLayer(pMaterialLayer),
  m_pLayer(pLayer)
{
}

void MaterialLayerPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pMaterial = m_pMaterialLayer->GetMaterial();
  QString materialName = pMaterial != nullptr ? QString::fromWCharArray(pMaterial->Name) : QApplication::translate("me_materialLayer", "noMaterial");
  mngr.addValue(propertyList, QApplication::translate("me_materialLayer", "material"), materialName);
}

void MaterialLayerPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pQuantities = m_pLayer->GetQuantities();
  auto properties = createQuantitiesInternal(mngr, *pQuantities);
  propertyList.splice(propertyList.end(), properties);
}
