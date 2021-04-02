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

void MaterialLayerPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pMaterialLayer = m_materialLayerAccess();
  if (pMaterialLayer == nullptr)
    return;

  auto pMaterial = pMaterialLayer->GetMaterial();
  QString materialName = pMaterial != nullptr ? QString::fromWCharArray(pMaterial->Name) : QApplication::translate("me_materialLayer", "noMaterial");
  mngr.addValue(propertyList, QApplication::translate("me_materialLayer", "material"), materialName);
}

void MaterialLayerPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pLayer = m_layerAccess();
  if (pLayer == nullptr)
    return;

  auto pQuantities = pLayer->GetQuantities();
  auto properties = createQuantitiesInternal(mngr, *pQuantities);
  propertyList.splice(propertyList.end(), properties);
}
