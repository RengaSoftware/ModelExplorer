//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyView.h"
#include "ObjectPropertyViewBuilderFactory.h"
#include "GuidUtils.h"


ObjectPropertyView::ObjectPropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication) :
  QtTreePropertyBrowser(pParent),
  m_pApplication(pApplication),
  m_currentObjectId(0),
  m_propertyViewMode(CategoryMode)
{
  m_pGroupManager = new QtGroupPropertyManager(this);
  initPropertyManagers();
}

void ObjectPropertyView::setSelectedObjectId(const int objId)
{
  m_currentObjectId = objId;
  buildPropertyView();
}

void ObjectPropertyView::changeMode(ObjectPropertyView::Mode newMode)
{
  m_propertyViewMode = newMode;
  buildPropertyView();
}

void ObjectPropertyView::initPropertyManagers()
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

void ObjectPropertyView::clearPropertyManagers()
{
  m_propertyManagers.m_pIntManager->clear();
  m_propertyManagers.m_pDoubleManager->clear();
  m_propertyManagers.m_pDoubleUserAttributeManager->clear();
  m_propertyManagers.m_pStringManager->clear();
  m_propertyManagers.m_pStringUserAttributeManager->clear();
  m_pGroupManager->clear();
}

void ObjectPropertyView::buildPropertyViewAsList(PropertyList& parameters, PropertyList& calculated, PropertyList& userDefinedProperties)
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

void ObjectPropertyView::buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties)
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

void ObjectPropertyView::buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& userDefinedProperties)
{
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object parameters"), parameters);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Calculated characteristics"), calculated);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object properties"), userDefinedProperties);
}

bool ObjectPropertyView::createProperties(PropertyList& parameters, PropertyList& calculated, PropertyList& userDefinedProperties)
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();
  auto pSelectedObject = pModelObjectCollection->GetObjectById(m_currentObjectId);

  // check if object was found in collection
  if (pSelectedObject == nullptr)
    return false;

  ObjectPropertyViewBuilderFactory propertyBuilderFactory(&m_propertyManagers, m_pApplication);
  std::unique_ptr<IObjectPropertyViewBuilder> objectPropertyViewBuilder(propertyBuilderFactory.createBuilder(pSelectedObject));

  parameters.clear();
  calculated.clear();

  objectPropertyViewBuilder->createParametersProperties(parameters, pSelectedObject);
  objectPropertyViewBuilder->createQuantitiesProperties(calculated, pSelectedObject);
  userDefinedProperties = objectPropertyViewBuilder->createUserAttributesProperties(pSelectedObject);
  return true;
}

void ObjectPropertyView::buildPropertyView()
{
  clearPropertyManagers();

  PropertyList parameters, calculated, userDefinedProperties;
  if(createProperties(parameters, calculated, userDefinedProperties) == false)
    return;

  if (m_propertyViewMode == Mode::ListMode)
    buildPropertyViewAsList(parameters, calculated, userDefinedProperties);
  else
    buildPropertyViewByCategory(parameters, calculated, userDefinedProperties);
}

void ObjectPropertyView::userDoubleAttributeChanged(QtProperty* userAttributeProperty, const QString& newValue)
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

void ObjectPropertyView::userStringAttributeChanged(QtProperty* userAttributeProperty, const QString& newValue)
{
  if (newValue.isEmpty())
    resetUserAttribute(userAttributeProperty); // will reset the attribute value
  else
    changeUserAttribute(userAttributeProperty, newValue.toStdWString());
}

Renga::IPropertyPtr ObjectPropertyView::getProperty(QtProperty* userAttributeProperty)
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();
  auto pSelectedObject = pModelObjectCollection->GetObjectById(m_currentObjectId);
  if (!pSelectedObject)
    return nullptr;

  const auto propertyId = GuidFromString(userAttributeProperty->data().toStdString());

  auto pProperties = pSelectedObject->GetProperties();
  return pProperties->GetProperty(propertyId);
}

Renga::IOperationPtr ObjectPropertyView::createOperation()
{
  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  return pModel->CreateOperation();
}

void ObjectPropertyView::resetUserAttribute(QtProperty* userAttributeProperty)
{
  auto pProperty = getProperty(userAttributeProperty);
  if (!pProperty)
    return;

  auto pOperation = createOperation();
  pOperation->Start();

  pProperty->ResetValue();

  pOperation->Apply();
}

void ObjectPropertyView::changeUserAttribute(QtProperty* userAttributeProperty, const double value)
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

void ObjectPropertyView::changeUserAttribute(QtProperty* userAttributeProperty, const std::wstring& value)
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
