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

#include <qteditorfactory.h>

ObjectPropertyView::ObjectPropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication) :
  QtTreePropertyBrowser(pParent),
  m_pApplication(pApplication),
  m_propertyViewMode(CategoryMode),
  m_pSelectedModelObject(nullptr),
  m_pSelectedMaterialLayer(nullptr),
  m_pSelectedLayer(nullptr),
  m_pSelectedRebarUsage(nullptr),
  m_pSelectedReinforcementUnitUsage(nullptr)
{
  m_pGroupManager = new QtGroupPropertyManager(this);

  initPropertyManagers();

  m_pObjectPropertyViewBuilderFactory = std::make_unique<ObjectPropertyViewBuilderFactory>(&m_propertyManagers, m_pApplication);
}

void ObjectPropertyView::showModelObjectProperties(Renga::IModelObjectPtr pModelObject)
{
  clearSelectedObjects();

  m_pSelectedModelObject = pModelObject;

  buildPropertyViewForModelObject(pModelObject);
}

void ObjectPropertyView::showMaterialLayerProperties(Renga::IMaterialLayerPtr pMaterialLayer, Renga::ILayerPtr pLayer)
{
  clearSelectedObjects();

  m_pSelectedMaterialLayer = pMaterialLayer;
  m_pSelectedLayer = pLayer;
  
  buildPropertyViewForMaterialLayer(pMaterialLayer, pLayer);
}

void ObjectPropertyView::showRebarUsageProperties(Renga::IRebarUsagePtr pRebarUsage)
{
  clearSelectedObjects();

  m_pSelectedRebarUsage = pRebarUsage;

  buildPropertyViewForRebarUsage(pRebarUsage);
}

void ObjectPropertyView::showReinforcementUnitUsageProperties(Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage)
{
  clearSelectedObjects();

  m_pSelectedReinforcementUnitUsage = pReinforcementUnitUsage;

  buildPropertyViewForReinforcementUnitUsage(pReinforcementUnitUsage);
}

void ObjectPropertyView::changeMode(ObjectPropertyView::Mode newMode)
{
  m_propertyViewMode = newMode;
  rebuildPropertyViewForSelectedObject();
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

void ObjectPropertyView::buildPropertyView(PropertyList& parameters, PropertyList& calculated, PropertyList& properties)
{
  if (m_propertyViewMode == Mode::ListMode)
    buildPropertyViewAsList(parameters, calculated, properties);
  else
    buildPropertyViewByCategory(parameters, calculated, properties);
}

void ObjectPropertyView::buildPropertyViewForModelObject(Renga::IModelObjectPtr pModelObject)
{
  clearPropertyManagers();

  PropertyList parameters;
  PropertyList calculated;
  PropertyList userDefinedProperties;

  if (createPropertiesForModelObject(pModelObject, parameters, calculated, userDefinedProperties))
    buildPropertyView(parameters, calculated, userDefinedProperties);
}

void ObjectPropertyView::buildPropertyViewForMaterialLayer(
  Renga::IMaterialLayerPtr pMaterialLayer,
  Renga::ILayerPtr pLayer)
{
  clearPropertyManagers();

  PropertyList parameters;
  PropertyList calculated;
  PropertyList userDefinedProperties;

  if (createPropertiesForMaterialLayer(pMaterialLayer, pLayer, parameters, calculated, userDefinedProperties))
    buildPropertyView(parameters, calculated, userDefinedProperties);
}

void ObjectPropertyView::buildPropertyViewForRebarUsage(Renga::IRebarUsagePtr pRebarUsage)
{
  clearPropertyManagers();

  PropertyList parameters;
  PropertyList calculated;
  PropertyList userDefinedProperties;

  if (createPropertiesForRebarUsage(pRebarUsage, parameters, calculated, userDefinedProperties))
    buildPropertyView(parameters, calculated, userDefinedProperties);
}

void ObjectPropertyView::buildPropertyViewForReinforcementUnitUsage(
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage)
{
  clearPropertyManagers();

  PropertyList parameters;
  PropertyList calculated;
  PropertyList userDefinedProperties;

  if (createPropertiesForReinforcementUnitUsage(pReinforcementUnitUsage, parameters, calculated, userDefinedProperties))
    buildPropertyView(parameters, calculated, userDefinedProperties);
}

void ObjectPropertyView::clearSelectedObjects()
{
  m_pSelectedModelObject = nullptr;
  m_pSelectedMaterialLayer = nullptr;
  m_pSelectedLayer = nullptr;
  m_pSelectedRebarUsage = nullptr;
  m_pSelectedReinforcementUnitUsage = nullptr;
}

void ObjectPropertyView::rebuildPropertyViewForSelectedObject()
{
  if (m_pSelectedModelObject != nullptr)
  {
    buildPropertyViewForModelObject(m_pSelectedModelObject);
  }
  else if (m_pSelectedMaterialLayer != nullptr && m_pSelectedLayer != nullptr)
  {
    buildPropertyViewForMaterialLayer(m_pSelectedMaterialLayer, m_pSelectedLayer);
  }
  else if (m_pSelectedRebarUsage != nullptr)
  {
    buildPropertyViewForRebarUsage(m_pSelectedRebarUsage);
  }
  else if (m_pSelectedReinforcementUnitUsage != nullptr)
  {
    buildPropertyViewForReinforcementUnitUsage(m_pSelectedReinforcementUnitUsage);
  }
  else
  {
    clearSelectedObjects();
    clearPropertyManagers();
  }
}

void ObjectPropertyView::buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& userDefinedProperties)
{
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object parameters"), parameters);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Calculated characteristics"), calculated);
  buildPropertyViewSingleCategory(QApplication::translate("me_propertyView", "Object properties"), userDefinedProperties);
}

bool ObjectPropertyView::createPropertiesForModelObject(
  Renga::IModelObjectPtr pModelObject,
  PropertyList& parameters,
  PropertyList& calculated,
  PropertyList& userDefinedProperties)
{
  std::unique_ptr<IObjectPropertyViewBuilder> pObjectPropertyViewBuilder(
    m_pObjectPropertyViewBuilderFactory->createBuilderForModelObject(pModelObject));

  if (pObjectPropertyViewBuilder == nullptr)
    return false;

  return createProperties(pObjectPropertyViewBuilder.get(), parameters, calculated, userDefinedProperties);
}

bool ObjectPropertyView::createPropertiesForMaterialLayer(
  Renga::IMaterialLayerPtr pMaterialLayer,
  Renga::ILayerPtr pLayer,
  PropertyList& parameters,
  PropertyList& calculated,
  PropertyList& userDefinedProperties)
{
  std::unique_ptr<IObjectPropertyViewBuilder> pObjectPropertyViewBuilder(
    m_pObjectPropertyViewBuilderFactory->createBuilderForMaterialLayer(pMaterialLayer, pLayer));

  if (pObjectPropertyViewBuilder == nullptr)
    return false;
  
  return createProperties(pObjectPropertyViewBuilder.get(), parameters, calculated, userDefinedProperties);
}

bool ObjectPropertyView::createPropertiesForRebarUsage(
  Renga::IRebarUsagePtr pRebarUsage,
  PropertyList& parameters,
  PropertyList& calculated,
  PropertyList& userDefinedProperties)
{
  std::unique_ptr<IObjectPropertyViewBuilder> pObjectPropertyViewBuilder(
    m_pObjectPropertyViewBuilderFactory->createBuilderForRebarUsage(pRebarUsage));

  if (pObjectPropertyViewBuilder == nullptr)
    return false;
  
  return createProperties(pObjectPropertyViewBuilder.get(), parameters, calculated, userDefinedProperties);
}

bool ObjectPropertyView::createPropertiesForReinforcementUnitUsage(
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage,
  PropertyList& parameters,
  PropertyList& calculated,
  PropertyList& userDefinedProperties)
{
  std::unique_ptr<IObjectPropertyViewBuilder> pObjectPropertyViewBuilder(
    m_pObjectPropertyViewBuilderFactory->createBuilderForReinforcementUnitUsage(pReinforcementUnitUsage));

  if (pObjectPropertyViewBuilder == nullptr)
    return false;

  return createProperties(pObjectPropertyViewBuilder.get(), parameters, calculated, userDefinedProperties);
}

bool ObjectPropertyView::createProperties(
  IObjectPropertyViewBuilder* pObjectPropertyViewBuilder,
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

//void ObjectPropertyView::buildPropertyView()
//{
//  clearPropertyManagers();
//
//  PropertyList parameters, calculated, userDefinedProperties;
//  if(createProperties(parameters, calculated, userDefinedProperties) == false)
//    return;
//
//  if (m_propertyViewMode == Mode::ListMode)
//    buildPropertyViewAsList(parameters, calculated, userDefinedProperties);
//  else
//    buildPropertyViewByCategory(parameters, calculated, userDefinedProperties);
//}

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
  if (m_pSelectedModelObject == nullptr)
    return nullptr;

  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();
  
  const auto propertyId = GuidFromString(userAttributeProperty->data().toStdString());

  auto pProperties = m_pSelectedModelObject->GetProperties();
  return pProperties->Get(propertyId);
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
