//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RengaPropertyController.h"

#include "COMUtils.h"

#include <qtpropertybrowser.h>


RengaPropertyController::RengaPropertyController(
    Renga::IApplicationPtr pRenga,
    PropertyContainerAccess propertiesAccess)
  : m_pRenga(pRenga),
    m_propertiesAccess(propertiesAccess)
{
}

void RengaPropertyController::onDoublePropertyChanged(QtProperty* pQtProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetPropertyValue(pQtProperty); // will reset the attribute value
  else
  {
    bool ok = false;
    double newDoubleValue = QLocale::system().toDouble(newValue, &ok);
    if (ok)
      changePropertyValue(pQtProperty, newDoubleValue);
  }
}

void RengaPropertyController::onStringPropertyChanged(QtProperty* pQtProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetPropertyValue(pQtProperty); // will reset the attribute value
  else
    changePropertyValue(pQtProperty, newValue);
}

void RengaPropertyController::onIntPropertyChanged(QtProperty* pQtProperty, int value)
{
  changePropertyValue(pQtProperty, value);
}

void RengaPropertyController::onBoolPropertyChanged(QtProperty* pQtProperty, bool value)
{
  changePropertyValue(pQtProperty, value);
}

Renga::IPropertyPtr RengaPropertyController::getProperty(QtProperty* pQtProperty)
{
  auto properties = m_propertiesAccess();
  if (properties == nullptr)
    return nullptr;

  const auto propertyId = GuidFromString(pQtProperty->data().toStdString());
  return properties->Get(propertyId);
}

Renga::IOperationPtr RengaPropertyController::createOperation()
{
  auto pProject = m_pRenga->GetProject();
  auto pModel = pProject->GetModel();
  return pModel->CreateOperation();
}

bool RengaPropertyController::tryGetEnumValueByIndex(GUID propertyId, int index, QString& result)
{
  auto pProject = m_pRenga->GetProject();
  if (pProject == nullptr)
    return false;

  auto pDesc = pProject->PropertyManager->GetPropertyDescription2(propertyId);
  auto enumValueList = safeArrayToQStringList(pDesc->GetEnumerationItems());
  if (index >= enumValueList.size())
    return false;

  result = enumValueList[index];
  return true;
}

void RengaPropertyController::resetPropertyValue(QtProperty* pQtProperty)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();
  pOperation->Start();

  pProperty->ResetValue();

  pOperation->Apply();
}

void RengaPropertyController::changePropertyValue(QtProperty* pQtProperty, const double value)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();
  pOperation->Start();

  switch (pProperty->GetType())
  {
  case Renga::PropertyType::PropertyType_Angle:
    pProperty->SetAngleValue(value, Renga::AngleUnit::AngleUnit_Degrees);
    break;
  case Renga::PropertyType::PropertyType_Length:
    pProperty->SetLengthValue(value, Renga::LengthUnit::LengthUnit_Millimeters);
    break;
  case Renga::PropertyType::PropertyType_Area:
    pProperty->SetAreaValue(value, Renga::AreaUnit::AreaUnit_Meters2);
    break;
  case Renga::PropertyType::PropertyType_Volume:
    pProperty->SetVolumeValue(value, Renga::VolumeUnit::VolumeUnit_Meters3);
    break;
  case Renga::PropertyType::PropertyType_Mass:
    pProperty->SetMassValue(value, Renga::MassUnit::MassUnit_Kilograms);
    break;
  case Renga::PropertyType::PropertyType_Double:
    pProperty->SetDoubleValue(value);
    break;
  default: break;
  }
  pOperation->Apply();
}

void RengaPropertyController::changePropertyValue(QtProperty* pQtProperty, const QString& value)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();

  pOperation->Start();

  switch (pProperty->GetType())
  {
  case Renga::PropertyType::PropertyType_String:
    pProperty->SetStringValue(value.toStdWString().c_str());
    break;
  default: break;
  }

  pOperation->Apply();
}

void RengaPropertyController::changePropertyValue(QtProperty* pQtProperty, int value)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();

  pOperation->Start();

  switch (pProperty->GetType())
  {
  case Renga::PropertyType::PropertyType_Integer:
    pProperty->SetIntegerValue(value);
    break;
  case Renga::PropertyType::PropertyType_Enumeration:
  {
    auto enumValue = QString();
    if (tryGetEnumValueByIndex(pProperty->Id, value, enumValue))
      pProperty->SetEnumerationValue(_bstr_t(enumValue.toStdWString().c_str()));
  }
  break;
  case Renga::PropertyType::PropertyType_Logical:
  {
    auto logicalValue = getLogicalValueFromIndex(value);
    pProperty->SetLogicalValue(logicalValue);
  }
  break;
  default:
    break;
  }
  pOperation->Apply();
}

void RengaPropertyController::changePropertyValue(QtProperty* pQtProperty, bool value)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();

  pOperation->Start();
  pProperty->SetBooleanValue(value);
  pOperation->Apply();
}
