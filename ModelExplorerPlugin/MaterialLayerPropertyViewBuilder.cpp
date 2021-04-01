#include "stdafx.h"

#include "MaterialLayerPropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


MaterialLayerPropertyViewBuilder::MaterialLayerPropertyViewBuilder(
    Renga::IApplicationPtr pApplication,
    MaterialLayerAccess materialLayerAccess,
    LayerAccess layerAccess)
  : m_pApplication(pApplication),
    m_materialLayerAccess(materialLayerAccess),
    m_layerAccess(layerAccess)
{
}

PropertyList MaterialLayerPropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  auto result = PropertyList();
  auto pMaterialLayer = m_materialLayerAccess();
  if (pMaterialLayer == nullptr)
    return result;
  
  auto pMaterial = pMaterialLayer->GetMaterial();
  QString materialName = pMaterial != nullptr ? QString::fromWCharArray(pMaterial->Name) : QApplication::translate("me_materialLayer", "noMaterial");
  mngr.addValue(result, QApplication::translate("me_materialLayer", "material"), materialName);
  return result;
}

PropertyList MaterialLayerPropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  auto pLayer = m_layerAccess();
  if (pLayer == nullptr)
    return {};

  auto pQuantities = pLayer->GetQuantities();
  return createQuantitiesInternal(mngr, *pQuantities);
}
