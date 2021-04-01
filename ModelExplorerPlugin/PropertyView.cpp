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

#include <Windows.h>


namespace
{
  bool createProperties(
    IPropertyViewBuilder* pObjectPropertyViewBuilder,
    PropertyManager& quantitiesMng,
    PropertyManager& parametersMng,
    PropertyManager& propertiesMng,
    PropertyList& parameters,
    PropertyList& quantities,
    PropertyList& properties)
  {
    parameters.clear();
    quantities.clear();

    

    return true;
  }

  void buildPropertyViewAsList(
    QtTreePropertyBrowser& propertyBrowser,
    PropertyList &parameters,
    PropertyList &calculated, 
    PropertyList &userDefinedProperties) 
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
      propertyBrowser.addProperty(*it);
  }

  void buildPropertyViewSingleCategory(
    QtTreePropertyBrowser& propertyBrowser, 
    QtGroupPropertyManager &groupManager,
    const QString& categoryName, 
    const PropertyList& categoryProperties)
  {
    if (categoryProperties.empty())
      return;

    QtProperty* pCategoryProperty = groupManager.addProperty(categoryName);
    pCategoryProperty->setBold(true);

    PropertyList singleCategoryProperties(categoryProperties);
    singleCategoryProperties.sort([](QtProperty* left, QtProperty* right) -> bool
    {
      return left->propertyName() < right->propertyName();
    });

    for (auto it = singleCategoryProperties.begin(); it != singleCategoryProperties.end(); ++it)
      pCategoryProperty->addSubProperty(*it);

    propertyBrowser.addProperty(pCategoryProperty);
  }

  void buildPropertyViewByCategory(
    QtTreePropertyBrowser& propertyBrowser, 
    QtGroupPropertyManager &groupManager,
    PropertyList& parameters, 
    const PropertyList& quantities, 
    const PropertyList& properties)
  {
    buildPropertyViewSingleCategory(propertyBrowser, groupManager, QApplication::translate("me_propertyView", "Parameters"), parameters);
    buildPropertyViewSingleCategory(propertyBrowser, groupManager, QApplication::translate("me_propertyView", "Calculated characteristics"), quantities);
    buildPropertyViewSingleCategory(propertyBrowser, groupManager, QApplication::translate("me_propertyView", "Properties"), properties);
  }

  void buildPropertyView(QtTreePropertyBrowser& propertyBrowser, 
                         PropertyManager& quantitiesMng,
                         PropertyManager& parametersMng,
                         PropertyManager& propertiesMng,
                         IPropertyViewBuilder& propertyViewBuilder,
                         QtGroupPropertyManager &groupManager,
                         PropertyView::Mode propertyViewMode)
  {
    PropertyList parameters = propertyViewBuilder.createParameters(parametersMng);
    PropertyList quantities = propertyViewBuilder.createQuantities(quantitiesMng);
    PropertyList properties = propertyViewBuilder.createProperties(propertiesMng);

    if (propertyViewMode == PropertyView::Mode::ListMode)
      buildPropertyViewAsList(propertyBrowser, parameters, quantities, properties);
    else
      buildPropertyViewByCategory(propertyBrowser, groupManager, parameters, quantities, properties);
  }
}

PropertyView::PropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication) :
  QtTreePropertyBrowser(pParent),
  m_pApplication(pApplication),
  m_propertyViewMode(CategoryMode)
{
  m_pGroupManager = new QtGroupPropertyManager(this);

  initPropertyManagers();
}

void PropertyView::showProperties(
    std::unique_ptr<IPropertyViewBuilder> builder,
    PropertyContainerAccess propertiesAccess)
{
  m_builder.swap(builder);
  m_propertiesAccess = propertiesAccess;

  clearPropertyManagers();
  buildPropertyView(
      *this,
      m_quantitiesMng,
      m_parametersMng,
      m_propertiesMng,
      *m_builder,
      *m_pGroupManager,
      m_propertyViewMode);
}

void PropertyView::changeMode(PropertyView::Mode newMode)
{
  m_propertyViewMode = newMode;

  clearPropertyManagers();
  buildPropertyView(
      *this,
      m_quantitiesMng,
      m_parametersMng,
      m_propertiesMng,
      *m_builder,
      *m_pGroupManager,
      m_propertyViewMode);
}

void PropertyView::initPropertyManagers()
{
  m_quantitiesMng.init(this, true);
  m_parametersMng.init(this, true);
  m_propertiesMng.init(this, false);

  // TODO: connect after property view was built
  {
    QObject::connect(m_parametersMng.m_pBoolManager, SIGNAL(valueChanged(QtProperty*, bool)), this, SLOT(parameterBoolChanged(QtProperty*, bool)));
    QObject::connect(m_parametersMng.m_pIntManager, SIGNAL(valueChanged(QtProperty*, int)), this, SLOT(parameterIntChanged(QtProperty*, int)));
    QObject::connect(m_parametersMng.m_pDoubleManager, SIGNAL(valueChanged(QtProperty*, const QString&)), this, SLOT(parameterDoubleChanged(QtProperty*, const QString&)));
    QObject::connect(m_parametersMng.m_pStringManager, SIGNAL(valueChanged(QtProperty*, const QString&)), this, SLOT(parameterStringChanged(QtProperty*, const QString&)));
  }

   QObject::connect(
     m_propertiesMng.m_pDoubleManager, SIGNAL(valueChanged(QtProperty*, const QString&)),
     this, SLOT(userDoubleAttributeChanged(QtProperty*, const QString&)));

   QObject::connect(
     m_propertiesMng.m_pStringManager, SIGNAL(valueChanged(QtProperty*, const QString&)),
     this, SLOT(userStringAttributeChanged(QtProperty*, const QString&)));
}

void PropertyView::clearPropertyManagers()
{
  m_pGroupManager->clear();
  m_quantitiesMng.clear();
  m_parametersMng.clear();
  m_propertiesMng.clear();
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
  auto properties = m_propertiesAccess();
  if (properties == nullptr)
    return nullptr;

  const auto propertyId = GuidFromString(userAttributeProperty->data().toStdString());
  return properties->Get(propertyId);
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
