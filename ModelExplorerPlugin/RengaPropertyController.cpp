//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RengaPropertyController.h"

#include "GuidUtils.h"

#include <qtpropertybrowser.h>


RengaPropertyController::RengaPropertyController(
    Renga::IApplicationPtr pRenga,
    PropertyContainerAccess propertiesAccess)
  : m_pRenga(pRenga),
    m_propertiesAccess(propertiesAccess)
{
}

void RengaPropertyController::qtDoublePropertyChanged(QtProperty* pQtProperty, const QString& newValue)
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

void RengaPropertyController::qtStringPropertyChanged(QtProperty* pQtProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetPropertyValue(pQtProperty); // will reset the attribute value
  else
    changePropertyValue(pQtProperty, newValue.toStdWString());
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

  if (pProperty->GetType() != Renga::PropertyType::PropertyType_Double)
    return;

  auto pOperation = createOperation();

  pOperation->Start();
  pProperty->SetDoubleValue(value);
  pOperation->Apply();
}

void RengaPropertyController::changePropertyValue(QtProperty* pQtProperty, const std::wstring& value)
{
  auto pProperty = getProperty(pQtProperty);
  if (!pProperty)
    return;

  if (pProperty->GetType() != Renga::PropertyType::PropertyType_String)
    return;

  auto pOperation = createOperation();

  pOperation->Start();
  pProperty->SetStringValue(value.c_str());
  pOperation->Apply();
}
