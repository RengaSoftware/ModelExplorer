//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyManagers.h"
#include "IObjectPropertyViewBuilder.h"
#include "ObjectPropertyViewBuilderFactory.h"

#include <qttreepropertybrowser.h>

const uint c_defaultPrecision = 2;
const uint c_userAttrPrecision = 15;

class ObjectPropertyView : public QtTreePropertyBrowser
{
  Q_OBJECT

public:
  enum Mode
  {
    CategoryMode = 0,
    ListMode = 1,
  };

  ObjectPropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication);
  
  void showModelObjectProperties(Renga::IModelObjectPtr pModelObject);
  void showMaterialLayerProperties(Renga::IMaterialLayerPtr pMaterialLayer, Renga::ILayerPtr pLayer);
  void showRebarUsageProperties(Renga::IRebarUsagePtr pRebarUsage);
  void showReinforcementUnitUsageProperties(Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void changeMode(ObjectPropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  
  bool createPropertiesForModelObject(
    Renga::IModelObjectPtr pModelObject,
    PropertyList& parameters,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);

  bool createPropertiesForMaterialLayer(
    Renga::IMaterialLayerPtr pMaterialLayer,
    Renga::ILayerPtr pLayer,
    PropertyList& parameters,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);

  bool createPropertiesForRebarUsage(
    Renga::IRebarUsagePtr pRebarUsage,
    PropertyList& parameters,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);
  
  bool createPropertiesForReinforcementUnitUsage(
    Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage,
    PropertyList& parameters,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);

  bool createProperties(
    IObjectPropertyViewBuilder* pObjectPropertyViewBuilder,
    PropertyList& parameters,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);

  void buildPropertyViewAsList(PropertyList& parameters, PropertyList& calculated, PropertyList& properties);
  void buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& properties);
  void buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties);

  void buildPropertyView(PropertyList& parameters, PropertyList& calculated, PropertyList& properties);

  void buildPropertyViewForModelObject(Renga::IModelObjectPtr pModelObject);
  void buildPropertyViewForMaterialLayer(Renga::IMaterialLayerPtr pMaterialLayer, Renga::ILayerPtr pLayer);
  void buildPropertyViewForRebarUsage(Renga::IRebarUsagePtr pRebarUsage);
  void buildPropertyViewForReinforcementUnitUsage(Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  void clearSelectedObjects();

  void rebuildPropertyViewForSelectedObject();

  Renga::IPropertyPtr getProperty(QtProperty* property);
  Renga::IOperationPtr createOperation();

  void resetUserAttribute(QtProperty* property);
  void changeUserAttribute(QtProperty* property, const double value);
  void changeUserAttribute(QtProperty* property, const std::wstring& value);

private:
  Renga::IApplicationPtr m_pApplication;
  std::unique_ptr<ObjectPropertyViewBuilderFactory> m_pObjectPropertyViewBuilderFactory;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManagers m_propertyManagers;
  Mode m_propertyViewMode;
  
  Renga::IModelObjectPtr m_pSelectedModelObject;
  Renga::IMaterialLayerPtr m_pSelectedMaterialLayer;
  Renga::ILayerPtr m_pSelectedLayer;
  Renga::IRebarUsagePtr m_pSelectedRebarUsage;
  Renga::IReinforcementUnitUsagePtr m_pSelectedReinforcementUnitUsage;
};
