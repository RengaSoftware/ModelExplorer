//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "PropertyView.h"
#include "COMUtils.h"

#include <qteditorfactory.h>

#include <Windows.h>


namespace
{
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
    propertyViewBuilder.createParameters(parametersMng);
    propertyViewBuilder.createQuantities(quantitiesMng);
    propertyViewBuilder.createProperties(propertiesMng);

    if (propertyViewMode == PropertyView::Mode::ListMode)
      buildPropertyViewAsList(
          propertyBrowser,
          parametersMng.properties(),
          quantitiesMng.properties(),
          propertiesMng.properties());
    else
      buildPropertyViewByCategory(
          propertyBrowser,
          groupManager,
          parametersMng.properties(),
          quantitiesMng.properties(),
          propertiesMng.properties());
  }
}

PropertyView::PropertyView(QWidget* pParent, Renga::IApplicationPtr pRenga) :
  QtTreePropertyBrowser(pParent),
  m_pRenga(pRenga),
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
  m_pPropertyController = std::make_unique<RengaPropertyController>(m_pRenga, propertiesAccess);

  clearPropertyManagers();
  buildPropertyView(
      *this,
      m_quantitiesMng,
      m_parametersMng,
      m_propertiesMng,
      *m_builder,
      *m_pGroupManager,
      m_propertyViewMode);

  QObject::connect(
      m_propertiesMng.m_pDoubleManager,
      SIGNAL(valueChanged(QtProperty*, const QString&)),
      m_pPropertyController.get(),
      SLOT(onDoublePropertyChanged(QtProperty*, const QString&)));

  QObject::connect(
      m_propertiesMng.m_pStringManager,
      SIGNAL(valueChanged(QtProperty*, const QString&)),
      m_pPropertyController.get(),
      SLOT(onStringPropertyChanged(QtProperty*, const QString&)));

  QObject::connect(
    m_propertiesMng.m_pIntManager,
    SIGNAL(valueChanged(QtProperty*, int)),
    m_pPropertyController.get(),
    SLOT(onIntPropertyChanged(QtProperty*, int)));

  QObject::connect(
    m_propertiesMng.m_pBoolManager,
    SIGNAL(valueChanged(QtProperty*, bool)),
    m_pPropertyController.get(),
    SLOT(onBoolPropertyChanged(QtProperty*, bool)));

  QObject::connect(
    m_propertiesMng.m_pEnumManager,
    SIGNAL(valueChanged(QtProperty*, int)),
    m_pPropertyController.get(),
    SLOT(onIntPropertyChanged(QtProperty*, int)));
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
}

void PropertyView::clearPropertyManagers()
{
  m_pGroupManager->clear();
  m_quantitiesMng.clear();
  m_parametersMng.clear();
  m_propertiesMng.clear();
}