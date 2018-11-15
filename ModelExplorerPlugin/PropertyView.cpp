//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "PropertyView.h"
#include "GuidUtils.h"

#include <qteditorfactory.h>
#include "PropertyViewModelObjectSource.h"

PropertyView::PropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication) :
  QtTreePropertyBrowser(pParent),
  m_pApplication(pApplication),
  m_propertyViewMode(CategoryMode)
{
  m_pGroupManager = new QtGroupPropertyManager(this);

  initPropertyManagers();
}

void PropertyView::showProperties(IPropertyViewSourceObject* pSourceObject)
{
  m_pSourceObject.reset(pSourceObject);

  buildPropertyView(pSourceObject);
}

void PropertyView::changeMode(PropertyView::Mode newMode)
{
  m_propertyViewMode = newMode;
  
  buildPropertyView(m_pSourceObject.get());
}

void PropertyView::initPropertyManagers()
{
  m_propertyManagers.m_pIntManager = new QtIntPropertyManager(this);
  m_propertyManagers.m_pDoubleManager = new QtStringPropertyManager(this, QtStringPropertyManager::valueTupe::doubleType, c_defaultPrecision, false);
  m_propertyManagers.m_pDoubleUserAttributeManager = new QtStringPropertyManager(this, QtStringPropertyManager::valueTupe::doubleType, c_userAttrPrecision, true);
  m_propertyManagers.m_pStringManager = new QtStringPropertyManager(this);
  m_propertyManagers.m_pStringUserAttributeManager = new QtStringPropertyManager(this);

  auto pIntReadOnlyPropertyFactory = new QtSpinBoxFactory(this, true);
  auto pDoublePropertyFactory = new QtLineEditFactory(this, false);
  auto pDoubleReadOnlyPropertyFactory = new QtLineEditFactory(this, true);
  auto pStringPropertyFactory = new QtLineEditFactory(this, false);
  auto pStringReadOnlyPropertyFactory = new QtLineEditFactory(this, true);

  setFactoryForManager(m_propertyManagers.m_pIntManager, pIntReadOnlyPropertyFactory);
  setFactoryForManager(m_propertyManagers.m_pDoubleManager, pDoubleReadOnlyPropertyFactory);
  setFactoryForManager(m_propertyManagers.m_pDoubleUserAttributeManager, pDoublePropertyFactory);
  setFactoryForManager(m_propertyManagers.m_pStringManager, pStringReadOnlyPropertyFactory);
  setFactoryForManager(m_propertyManagers.m_pStringUserAttributeManager, pStringPropertyFactory);

   // handle user attributes changes
   QObject::connect(
     m_propertyManagers.m_pDoubleUserAttributeManager, SIGNAL(valueChanged(QtProperty*, const QString&)),
     this, SLOT(userDoubleAttributeChanged(QtProperty*, const QString&)));
   QObject::connect(
     m_propertyManagers.m_pStringUserAttributeManager, SIGNAL(valueChanged(QtProperty*, const QString&)),
     this, SLOT(userStringAttributeChanged(QtProperty*, const QString&)));
}

void PropertyView::clearPropertyManagers()
{
  m_propertyManagers.m_pIntManager->clear();
  m_propertyManagers.m_pDoubleManager->clear();
  m_propertyManagers.m_pDoubleUserAttributeManager->clear();
  m_propertyManagers.m_pStringManager->clear();
  m_propertyManagers.m_pStringUserAttributeManager->clear();
  m_pGroupManager->clear();
}

void PropertyView::buildPropertyViewAsList(PropertyList& parameters, PropertyList& calculated, PropertyList& userDefinedProperties)
{
  // show in list mode
  PropertyList allProperties;
  allProperties.splice(allProperties.end(), parameters);
  allProperties.splice(allProperties.end(), calculated);
  allProperties.splice(allProperties.end(), userDefinedProperties);

  allProperties.sort([](QtProperty* left, QtProperty* right) -> bool
                    {
                    return left->propertyName() < right->propertyName();
                    });

  for (auto it = allProperties.begin(); it != allProperties.end(); ++it)
    addProperty(*it);
}

void PropertyView::buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties)
{
  if(categoryProperties.empty())
    return;

  QtProperty* pCategoryProperty = m_pGroupManager->addProperty(categoryName);
  pCategoryProperty->setBold(true);
  
  PropertyList singleCategoryProperties(categoryProperties);
  singleCategoryProperties.sort([](QtProperty* left, QtProperty* right) -> bool
                               {
                                 return left->propertyName() < right->propertyName();
                               });

  for (auto it = singleCategoryProperties.begin(); it != singleCategoryProperties.end(); ++it)
    pCategoryProperty->addSubProperty(*it);

  addProperty(pCategoryProperty);
}

void PropertyView::buildPropertyView(IPropertyViewSourceObject* pSourceObject)
{
  clearPropertyManagers();

  if (pSourceObject == nullptr)
    return;

  auto propertyViewBuilder = pSourceObject->createPropertyViewBuilder(&m_propertyManagers);

  PropertyList parameters;
  PropertyList calculated;
  PropertyList userDefinedProperties;

  if (!createProperties(propertyViewBuilder.get(), parameters, calculated, userDefinedProperties))
    return;

  if (m_propertyViewMode == Mode::ListMode)
    buildPropertyViewAsList(parameters, calculated, userDefinedProperties);
  else
    buildPropertyViewByCategory(parameters, calculated, userDefinedProperties);
}

void PropertyView::buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& userDefinedProperties)
{
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object parameters"), parameters);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Calculated characteristics"), calculated);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object properties"), userDefinedProperties);
}

bool PropertyView::createProperties(
  IPropertyViewBuilder* pObjectPropertyViewBuilder,
  PropertyList& parameters,
  PropertyList& calculated,
  PropertyList& userDefinedProperties)
{
  if (pObjectPropertyViewBuilder == nullptr)
    return false;

  parameters.clear();
  calculated.clear();

  pObjectPropertyViewBuilder->createParametersProperties(parameters);
  pObjectPropertyViewBuilder->createQuantitiesProperties(calculated);

  userDefinedProperties = pObjectPropertyViewBuilder->createUserAttributesProperties();

  return true;
}

void PropertyView::userDoubleAttributeChanged(QtProperty* userAttributeProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetUserAttribute(userAttributeProperty); // will reset the attribute value
  else
  {
    bool ok = false;
    double newDoubleValue = QLocale::system().toDouble(newValue, &ok);
    if (ok)
      changeUserAttribute(userAttributeProperty, newDoubleValue);
  }
}

void PropertyView::userStringAttributeChanged(QtProperty* userAttributeProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetUserAttribute(userAttributeProperty); // will reset the attribute value
  else
    changeUserAttribute(userAttributeProperty, newValue.toStdWString());
}

Renga::IPropertyPtr PropertyView::getProperty(QtProperty* userAttributeProperty)
{
  if (m_pSourceObject == nullptr)
    return nullptr;

  const auto propertyId = GuidFromString(userAttributeProperty->data().toStdString());
     
  return m_pSourceObject->getUserDefinedProperty(propertyId);
}

Renga::IOperationPtr PropertyView::createOperation()
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  return pModel->CreateOperation();
}

void PropertyView::resetUserAttribute(QtProperty* userAttributeProperty)
{
  auto pProperty = getProperty(userAttributeProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();
  pOperation->Start();

  pProperty->ResetValue();

  pOperation->Apply();
}

void PropertyView::changeUserAttribute(QtProperty* userAttributeProperty, const double value)
{
  auto pProperty = getProperty(userAttributeProperty);
  if (!pProperty)
    return;

  if (pProperty->GetType() != Renga::PropertyType::PropertyType_Double)
    return;

  auto pOperation = createOperation();

  pOperation->Start();
  pProperty->SetDoubleValue(value);
  pOperation->Apply();
}

void PropertyView::changeUserAttribute(QtProperty* userAttributeProperty, const std::wstring& value)
{
  auto pProperty = getProperty(userAttributeProperty);
  if (!pProperty)
    return;

  if (pProperty->GetType() != Renga::PropertyType::PropertyType_String)
    return;

  auto pOperation = createOperation();

  pOperation->Start();
  pProperty->SetStringValue(value.c_str());
  pOperation->Apply();
}
