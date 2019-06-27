#include "stdafx.h"

#include "MaterialLayerPropertyViewBuilder.h"

#include <Renga/QuantityIds.h>

MaterialLayerPropertyViewBuilder::MaterialLayerPropertyViewBuilder(
  const PropertyManagers* pPropertyManagers,
  Renga::IApplicationPtr pApplication,
  Renga::IMaterialLayerPtr pMaterialLayer,
  Renga::ILayerPtr pLayer) :
  m_pPropertyManagers(pPropertyManagers),
  m_pApplication(pApplication),
  m_pMaterialLayer(pMaterialLayer),
  m_pLayer(pLayer)
{
}

void MaterialLayerPropertyViewBuilder::createParametersProperties(PropertyList& propertyList)
{
  auto pMaterial = m_pMaterialLayer->GetMaterial();
  QString materialName = pMaterial != nullptr ? QString::fromWCharArray(pMaterial->Name) : QApplication::translate("me_materialLayer", "noMaterial");
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "material"), materialName);
}

void MaterialLayerPropertyViewBuilder::createParametersPropertiesEx(PropertyList& propertyList)
{
}

void MaterialLayerPropertyViewBuilder::createQuantitiesProperties(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pLayer->GetQuantities();

  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "nominalThickness"), pQuantities, QuantityIds::NominalThickness);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "nominalLength"), pQuantities, QuantityIds::NominalLength);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "nominalHeight"), pQuantities, QuantityIds::NominalHeight);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "netVolume"), pQuantities, QuantityIds::NetVolume);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "netMass"), pQuantities, QuantityIds::NetMass);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "netArea"), pQuantities, QuantityIds::NetArea);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "netFootprintArea"), pQuantities, QuantityIds::NetFootprintArea);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "netSideArea"), pQuantities, QuantityIds::NetSideArea);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_materialLayer", "totalSurfaceArea"), pQuantities, QuantityIds::TotalSurfaceArea);
}

PropertyList MaterialLayerPropertyViewBuilder::createUserAttributesProperties()
{
  return PropertyList();
}
