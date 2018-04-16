//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyViewBuilder.h"
#include "GuidUtils.h"

#include <Renga/QuantityIds.h>


ObjectPropertyViewBuilder::ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers, Renga::IApplicationPtr pApplication) :
  m_pApplication(pApplication),
  m_pPropertyManagers(pPropertyManagers)
{
}

void ObjectPropertyViewBuilder::createParametersProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject)
{
  addValue(propertyList, QApplication::translate("me_mo", "name"), QString::fromWCharArray(pObject->GetName()));

  Renga::ILevelObjectPtr pLevelObject;
  pObject->QueryInterface(&pLevelObject);
  if (pLevelObject)
    addValue(propertyList, QApplication::translate("me_mo", "offset"), pLevelObject->GetOffset());

  Renga::IObjectWithMarkPtr pObjectWithMark;
  pObject->QueryInterface(&pObjectWithMark);
  if (pObjectWithMark)
    addValue(propertyList, QApplication::translate("me_mo", "mark"), QString::fromWCharArray(pObjectWithMark->GetMark()));

  Renga::IObjectWithMaterialPtr pObjectWithMaterial;
  pObject->QueryInterface(&pObjectWithMaterial);
  if (pObjectWithMaterial)
    addValue(propertyList, QApplication::translate("me_mo", "material"), getMaterialName(pObjectWithMaterial->GetMaterialId()));

  Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
  pObject->QueryInterface(&pObjectWithLayeredMaterial);
  if (pObjectWithLayeredMaterial)
    addValue(propertyList, QApplication::translate("me_mo", "material"), getLayeredMaterialName(pObjectWithLayeredMaterial->GetLayeredMaterialId()));

  Renga::IObjectWithMaterialListPtr pObjectWithMaterialList;
  pObject->QueryInterface(&pObjectWithMaterialList);
  if (pObjectWithMaterialList)
    for (size_t i = 0; i < pObjectWithMaterialList->GetMaterialCount(); i++)
      addValue(propertyList, QApplication::translate("me_mo", "material"), getMaterialName(pObjectWithMaterialList->GetMaterialId(i)));
}

void ObjectPropertyViewBuilder::createQuantitiesProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject)
{
  using namespace Renga;

  auto pQuantities = pObject->GetQuantities();

  PropertyList result;

  addValue(propertyList, QApplication::translate("me_mo", "overallWidth"), pQuantities, QuantityIds::OverallWidth);
  addValue(propertyList, QApplication::translate("me_mo", "overallHeight"), pQuantities, QuantityIds::OverallHeight);
  addValue(propertyList, QApplication::translate("me_mo", "overallWidth"), pQuantities, QuantityIds::OverallWidth);

  addValue(propertyList, QApplication::translate("me_mo", "thickness"), pQuantities, QuantityIds::NominalThickness);
  addValue(propertyList, QApplication::translate("me_mo", "length"), pQuantities, QuantityIds::NominalLength);
  addValue(propertyList, QApplication::translate("me_mo", "width"), pQuantities, QuantityIds::NominalWidth);
  addValue(propertyList, QApplication::translate("me_mo", "height"), pQuantities, QuantityIds::NominalHeight);

  addValue(propertyList, QApplication::translate("me_mo", "perimeter"), pQuantities, QuantityIds::Perimeter);

  addValue(propertyList, QApplication::translate("me_mo", "volume"), pQuantities, QuantityIds::Volume);
  addValue(propertyList, QApplication::translate("me_mo", "netVolume"), pQuantities, QuantityIds::NetVolume);
  addValue(propertyList, QApplication::translate("me_mo", "mass"), pQuantities, QuantityIds::NetMass);

  addValue(propertyList, QApplication::translate("me_mo", "crossSectionOverallWidth"), pQuantities, QuantityIds::CrossSectionOverallWidth);
  addValue(propertyList, QApplication::translate("me_mo", "crossSectionOverallHeight"), pQuantities, QuantityIds::CrossSectionOverallHeight);
  addValue(propertyList, QApplication::translate("me_mo", "crossSectionArea"), pQuantities, QuantityIds::CrossSectionArea);

  addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceArea"), pQuantities, QuantityIds::InnerSurfaceArea);
  addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceInternalArea"), pQuantities, QuantityIds::InnerSurfaceInternalArea);
  addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceExternalArea"), pQuantities, QuantityIds::InnerSurfaceExternalArea);

  addValue(propertyList, QApplication::translate("me_mo", "outerSurfaceArea"), pQuantities, QuantityIds::OuterSurfaceArea);

  addValue(propertyList, QApplication::translate("me_mo", "area"), pQuantities, QuantityIds::Area);
  addValue(propertyList, QApplication::translate("me_mo", "netArea"), pQuantities, QuantityIds::NetArea);
  addValue(propertyList, QApplication::translate("me_mo", "netFloorArea"), pQuantities, QuantityIds::NetFloorArea);
  addValue(propertyList, QApplication::translate("me_mo", "netFootprintArea"), pQuantities, QuantityIds::NetFootprintArea);
  addValue(propertyList, QApplication::translate("me_mo", "netSideArea"), pQuantities, QuantityIds::NetSideArea);
  addValue(propertyList, QApplication::translate("me_mo", "totalSurfaceArea"), pQuantities, QuantityIds::TotalSurfaceArea);

  addValue(propertyList, QApplication::translate("me_mo", "grossVolume"), pQuantities, Renga::QuantityIds::GrossVolume);
  addValue(propertyList, QApplication::translate("me_mo", "grossFloorArea"), pQuantities, Renga::QuantityIds::GrossFloorArea);
  addValue(propertyList, QApplication::translate("me_mo", "grossPerimeter"), pQuantities, Renga::QuantityIds::GrossPerimeter);

  addValue(propertyList, QApplication::translate("me_mo", "numberOfRiser"), pQuantities, QuantityIds::NumberOfRiser);
  addValue(propertyList, QApplication::translate("me_mo", "numberOfTreads"), pQuantities, QuantityIds::NumberOfTreads);
  addValue(propertyList, QApplication::translate("me_mo", "riserHeight"), pQuantities, QuantityIds::RiserHeight);
  addValue(propertyList, QApplication::translate("me_mo", "treadLength"), pQuantities, QuantityIds::TreadLength);
}

PropertyList ObjectPropertyViewBuilder::createUserAttributesProperties(Renga::IModelObjectPtr pObject)
{
  PropertyList pResult;

  auto pProject = m_pApplication->GetProject();
  auto pPropertyManager = pProject->GetPropertyManager();

  // block signals before filling properties
  m_pPropertyManagers->m_pStringUserAttributeManager->blockSignals(true);
  m_pPropertyManagers->m_pDoubleUserAttributeManager->blockSignals(true);

  // check all attributes
  for (int i = 0; i < pPropertyManager->GetPropertyCount(); ++i)
  {
    auto propertyId = pPropertyManager->GetProperty(i);

    // check if objectType has attribute
    if (!pPropertyManager->IsPropertyAssignedToType(propertyId, pObject->GetObjectType()))
      continue;

    auto pProperties = pObject->GetProperties();
    auto pProperty = pProperties->GetProperty(propertyId);

    if (!pProperty)
      continue;

    auto propertyDescription = pPropertyManager->GetPropertyDescription(propertyId);
    if (propertyDescription.type == Renga::PropertyType::PropertyType_Undefined)
      continue;

    const auto attributeName = QString::fromWCharArray(propertyDescription.name);
    const auto propertyIdString = QString::fromStdString((GuidToString(propertyId)));

    QtProperty* pUserAttributeProperty(nullptr);
    switch (propertyDescription.type)
    {
    case Renga::PropertyType::PropertyType_Double:
      {
        auto pManager = m_pPropertyManagers->m_pDoubleUserAttributeManager;
        auto pQtProperty = pManager->addProperty(attributeName);
        pManager->setValue(pQtProperty, pProperty->GetDoubleValue());
        pUserAttributeProperty = pQtProperty;
      }
      break;
    case Renga::PropertyType::PropertyType_String:
      {
        auto pManager = m_pPropertyManagers->m_pStringUserAttributeManager;
        auto pQtProperty = pManager->addProperty(attributeName);
        pManager->setValue(pQtProperty, QString::fromWCharArray(pProperty->GetStringValue()));
        pUserAttributeProperty = pQtProperty;
      }
      break;
    default:
      assert(false);
    }
    pUserAttributeProperty->setModified(true);
    pUserAttributeProperty->setData(propertyIdString);

    pResult.push_back(pUserAttributeProperty);
  }

  // unblock signals
  m_pPropertyManagers->m_pDoubleUserAttributeManager->blockSignals(false);
  m_pPropertyManagers->m_pStringUserAttributeManager->blockSignals(false);

  return pResult;
}

QString ObjectPropertyViewBuilder::getMaterialName(const Renga::MaterialId& materialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pMaterialManager = pProject->GetMaterialManager();
  auto pMaterial = pMaterialManager->GetMaterial(materialId);
  if (!pMaterial)
    return QString();

  return QString::fromWCharArray(pMaterial->GetName());
}

QString ObjectPropertyViewBuilder::getLayeredMaterialName(const Renga::LayeredMaterialId& layeredMaterialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pLayeredMaterialManager = pProject->GetLayeredMaterialManager();
  auto pLayeredMaterial = pLayeredMaterialManager->GetLayeredMaterial(layeredMaterialId);
  if (!pLayeredMaterial)
    return QString();

  return QString::fromWCharArray(pLayeredMaterial->GetName());
}

void ObjectPropertyViewBuilder::addValue(PropertyList& propertyList, const QString& name, const int value) const
{
  m_pPropertyManagers->addValue(propertyList, name, value);
}

void ObjectPropertyViewBuilder::addValue(PropertyList& propertyList, const QString& name, const double value) const
{
  m_pPropertyManagers->addValue(propertyList, name, value);
}

void ObjectPropertyViewBuilder::addValue(PropertyList& propertyList, const QString& name, const QString& value) const
{
  m_pPropertyManagers->addValue(propertyList, name, value);
}

void ObjectPropertyViewBuilder::addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityContainerPtr pQuantityContainer, const GUID quantityId) const {
  m_pPropertyManagers->addValue(propertyList, name, pQuantityContainer, quantityId);
}

void ObjectPropertyViewBuilder::addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityPtr pQuantity) const
{
  m_pPropertyManagers->addValue(propertyList, name, pQuantity);
}
