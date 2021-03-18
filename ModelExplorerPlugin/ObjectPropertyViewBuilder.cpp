//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyViewBuilder.h"
#include "PropertyManager.h"
#include "GuidUtils.h"

#include <Renga/QuantityIds.h>


ObjectPropertyViewBuilder::ObjectPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                         Renga::IModelObjectPtr pModelObject) :
  m_pApplication(pApplication),
  m_pModelObject(pModelObject)
{
}

void ObjectPropertyViewBuilder::createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList)
{
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

void ObjectPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  // block signals before filling properties
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

      //TODO: [asv] to enable parameters update
      pQtProperty->setModified(false);

      const auto parameterIdString = QString::fromStdString((GuidToString(id)));
      pQtProperty->setData(parameterIdString);
    }
  }

  // unblock signals
  mngr.blockSignals(false);
}

void ObjectPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pQuantities = m_pModelObject->GetQuantities();
  auto properties = createQuantitiesInternal(mngr, *pQuantities);
  propertyList.splice(propertyList.end(), properties);
}

PropertyList ObjectPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  return createPropertiesInternal(mngr, m_pModelObject->GetProperties());
}

QString ObjectPropertyViewBuilder::getMaterialName(const int& materialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pMaterialManager = pProject->GetMaterialManager();
  auto pMaterial = pMaterialManager->GetMaterial(materialId);
  if (!pMaterial)
    return QString();

  return QString::fromWCharArray(pMaterial->GetName());
}

QString ObjectPropertyViewBuilder::getLayeredMaterialName(const int& layeredMaterialId)
{
  auto pProject = m_pApplication->GetProject();
  auto pLayeredMaterialManager = pProject->GetLayeredMaterialManager();
  auto pLayeredMaterial = pLayeredMaterialManager->GetLayeredMaterial(layeredMaterialId);
  if (!pLayeredMaterial)
    return QString();

  return QString::fromWCharArray(pLayeredMaterial->GetName());
}
