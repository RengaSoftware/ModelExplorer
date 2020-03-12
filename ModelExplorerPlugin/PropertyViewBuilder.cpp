//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "PropertyViewBuilder.h"
#include "GuidUtils.h"

#include <Renga/QuantityIds.h>


PropertyViewBuilder::PropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                         Renga::IApplicationPtr pApplication,
                                         Renga::IModelObjectPtr pModelObject) :
  m_pApplication(pApplication),
  m_pPropertyManagers(pPropertyManagers),
  m_pModelObject(pModelObject)
{
}

void PropertyViewBuilder::createIntegratedParameters(PropertyList& propertyList)
{
  auto& mngr = m_pPropertyManagers->m_default;

  mngr.addValue(propertyList, QApplication::translate("me_mo", "name"), QString::fromWCharArray(m_pModelObject->GetName()));

  Renga::ILevelObjectPtr pLevelObject;
  m_pModelObject->QueryInterface(&pLevelObject);
  if (pLevelObject)
  {
    mngr.addValue(propertyList, QApplication::translate("me_mo", "offset"), pLevelObject->GetVerticalOffset());
    //add
    //pLevelObject->GetPlacementElevation();
    //pLevelObject->GetElevationAboveLevel();
  }

  Renga::IObjectWithMarkPtr pObjectWithMark;
  m_pModelObject->QueryInterface(&pObjectWithMark);
  if (pObjectWithMark)
    mngr.addValue(propertyList, QApplication::translate("me_mo", "mark"), QString::fromWCharArray(pObjectWithMark->GetMark()));

  Renga::IObjectWithMaterialPtr pObjectWithMaterial;
  m_pModelObject->QueryInterface(&pObjectWithMaterial);
  if (pObjectWithMaterial)
    mngr.addValue(propertyList, QApplication::translate("me_mo", "material"), getMaterialName(pObjectWithMaterial->GetMaterialId()));

  Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
  m_pModelObject->QueryInterface(&pObjectWithLayeredMaterial);
  if (pObjectWithLayeredMaterial)
    mngr.addValue(propertyList, QApplication::translate("me_mo", "material"), getLayeredMaterialName(pObjectWithLayeredMaterial->GetLayeredMaterialId()));
}

void PropertyViewBuilder::createParameters(PropertyList& propertyList)
{
  // block signals before filling properties
  auto& mngr = m_pPropertyManagers->m_parameters;
  mngr.blockSignals(true);

  auto pParameters = m_pModelObject->GetParameters();
  auto pIds = pParameters->GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    const auto id = pIds->Get(i);

    auto pParameter = pParameters->Get(id);
    auto pDefinition = pParameter->Definition;

    QString name = QString::fromStdWString(pDefinition->Name.operator wchar_t *());

    switch (pDefinition->GetParameterType())
    {
    case Renga::ParameterType::ParameterType_Angle:
      name += ", " + QApplication::translate("me_mo", "angle_dimension");
      break;
    case Renga::ParameterType::ParameterType_Length:
      name += ", " + QApplication::translate("me_mo", "length_dimension");
      break;
    }

    QtProperty* pQtProperty(nullptr);
    switch (pParameter->GetValueType())
    {
    case Renga::ParameterValueType::ParameterValueType_Bool:
      pQtProperty = mngr.addValue(propertyList, name, static_cast<bool>(pParameter->GetBoolValue()));
      break;
    case Renga::ParameterValueType::ParameterValueType_Int:
      pQtProperty = mngr.addValue(propertyList, name, pParameter->GetIntValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_Double:
      pQtProperty = mngr.addValue(propertyList, name, pParameter->GetDoubleValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_String:
      pQtProperty = mngr.addValue(propertyList, name, QString::fromWCharArray(pParameter->GetStringValue()));
      break;
    }

    if (pQtProperty)
    {
      if (!pParameter->HasValue())
        pQtProperty->setEnabled(false);

      pQtProperty->setModified(true);

      const auto parameterIdString = QString::fromStdString((GuidToString(id)));
      pQtProperty->setData(parameterIdString);
    }
  }

  // unblock signals
  mngr.blockSignals(false);
}

void PropertyViewBuilder::createQuantities(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pModelObject->GetQuantities();

  PropertyList result;

  auto& mngr = m_pPropertyManagers->m_default;

  mngr.addValue(propertyList, QApplication::translate("me_mo", "overallWidth"), pQuantities->Get(QuantityIds::OverallWidth));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "overallHeight"), pQuantities->Get(QuantityIds::OverallHeight));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "overallLength"), pQuantities->Get(QuantityIds::OverallLength));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "thickness"), pQuantities->Get(QuantityIds::NominalThickness));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "length"), pQuantities->Get(QuantityIds::NominalLength));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "width"), pQuantities->Get(QuantityIds::NominalWidth));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "height"), pQuantities->Get(QuantityIds::NominalHeight));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "perimeter"), pQuantities->Get(QuantityIds::Perimeter));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "volume"), pQuantities->Get(QuantityIds::Volume));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "netVolume"), pQuantities->Get(QuantityIds::NetVolume));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "mass"), pQuantities->Get(QuantityIds::NetMass));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "crossSectionOverallWidth"), pQuantities->Get(QuantityIds::CrossSectionOverallWidth));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "crossSectionOverallHeight"), pQuantities->Get(QuantityIds::CrossSectionOverallHeight));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "crossSectionArea"), pQuantities->Get(QuantityIds::CrossSectionArea));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceArea"), pQuantities->Get(QuantityIds::InnerSurfaceArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceInternalArea"), pQuantities->Get(QuantityIds::InnerSurfaceInternalArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "innerSurfaceExternalArea"), pQuantities->Get(QuantityIds::InnerSurfaceExternalArea));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "outerSurfaceArea"), pQuantities->Get(QuantityIds::OuterSurfaceArea));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "area"), pQuantities->Get(QuantityIds::Area));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "netArea"), pQuantities->Get(QuantityIds::NetArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "netFloorArea"), pQuantities->Get(QuantityIds::NetFloorArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "netFootprintArea"), pQuantities->Get(QuantityIds::NetFootprintArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "netSideArea"), pQuantities->Get(QuantityIds::NetSideArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "totalSurfaceArea"), pQuantities->Get(QuantityIds::TotalSurfaceArea));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "grossVolume"), pQuantities->Get(Renga::QuantityIds::GrossVolume));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "grossFloorArea"), pQuantities->Get(Renga::QuantityIds::GrossFloorArea));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "grossPerimeter"), pQuantities->Get(Renga::QuantityIds::GrossPerimeter));

  mngr.addValue(propertyList, QApplication::translate("me_mo", "numberOfRiser"), pQuantities->Get(QuantityIds::NumberOfRiser));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "numberOfTreads"), pQuantities->Get(QuantityIds::NumberOfTreads));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "riserHeight"), pQuantities->Get(QuantityIds::RiserHeight));
  mngr.addValue(propertyList, QApplication::translate("me_mo", "treadLength"), pQuantities->Get(QuantityIds::TreadLength));

  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities->Get(QuantityIds::TotalRebarLength));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities->Get(QuantityIds::TotalRebarMass));
}

PropertyList PropertyViewBuilder::createProperties()
{
  PropertyList pResult;

  auto pProject = m_pApplication->GetProject();
  auto pPropertyManager = pProject->GetPropertyManager();

  // block signals before filling properties
  auto& mngr = m_pPropertyManagers->m_properties;
  mngr.blockSignals(true);

  // check all attributes
  for (int i = 0; i < pPropertyManager->GetPropertyCount(); ++i)
  {
    auto propertyId = pPropertyManager->GetPropertyId(i);

    // check if objectType has attribute
    if (!pPropertyManager->IsPropertyAssignedToType(propertyId, m_pModelObject->GetObjectType()))
      continue;

    auto pProperties = m_pModelObject->GetProperties();
    auto pProperty = pProperties->Get(propertyId);

    if (!pProperty)
      continue;

    auto propertyDescription = pPropertyManager->GetPropertyDescription(propertyId);
    if (propertyDescription.Type == Renga::PropertyType::PropertyType_Undefined)
      continue;

    const auto attributeName = QString::fromWCharArray(propertyDescription.Name);
    const auto propertyIdString = QString::fromStdString((GuidToString(propertyId)));

    QtProperty* pQtProperty(nullptr);
    switch (propertyDescription.Type)
    {
    case Renga::PropertyType::PropertyType_Double:
      pQtProperty = mngr.addValue(pResult, attributeName, pProperty->GetDoubleValue());
      break;
    case Renga::PropertyType::PropertyType_String:
      pQtProperty = mngr.addValue(pResult, attributeName, QString::fromWCharArray(pProperty->GetStringValue()));
      break;
    default:
      assert(false);
    }
    pQtProperty->setModified(true);
    pQtProperty->setData(propertyIdString);
  }

  // unblock signals
  mngr.blockSignals(false);

  return pResult;
}

QString PropertyViewBuilder::getMaterialName(const int& materialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pMaterialManager = pProject->GetMaterialManager();
  auto pMaterial = pMaterialManager->GetMaterial(materialId);
  if (!pMaterial)
    return QString();

  return QString::fromWCharArray(pMaterial->GetName());
}

QString PropertyViewBuilder::getLayeredMaterialName(const int& layeredMaterialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pLayeredMaterialManager = pProject->GetLayeredMaterialManager();
  auto pLayeredMaterial = pLayeredMaterialManager->GetLayeredMaterial(layeredMaterialId);
  if (!pLayeredMaterial)
    return QString();

  return QString::fromWCharArray(pLayeredMaterial->GetName());
}
