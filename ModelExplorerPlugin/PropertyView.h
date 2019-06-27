//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyManagers.h"
#include "IPropertyViewBuilder.h"
#include "IPropertyViewSourceObject.h"

#include <qttreepropertybrowser.h>

const uint c_defaultPrecision = 2;
const uint c_userAttrPrecision = 15;

class PropertyView : public QtTreePropertyBrowser
{
  Q_OBJECT
    
public:
  enum Mode
  {
    CategoryMode = 0,
    ListMode = 1,
  };

  PropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication);

  void showProperties(IPropertyViewSourceObject* pSourceObject);
  
  /*void showModelObjectProperties(Renga::IModelObjectPtr pModelObject);
  void showMaterialLayerProperties(Renga::IMaterialLayerPtr pMaterialLayer, Renga::ILayerPtr pLayer);
  void showRebarUsageProperties(Renga::IRebarUsagePtr pRebarUsage);
  void showReinforcementUnitUsageProperties(Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);*/

  void changeMode(PropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  
  /*bool createPropertiesForModelObject(
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
    PropertyList& userDefinedProperties);*/

  bool createProperties(
    IPropertyViewBuilder* pObjectPropertyViewBuilder,
    PropertyList& parameters,
    PropertyList& parametersEx,
    PropertyList& calculated,
    PropertyList& userDefinedProperties);

  void buildPropertyViewAsList(PropertyList& parameters, PropertyList& parametersEx, PropertyList& calculated, PropertyList& properties);
  void buildPropertyViewByCategory(const PropertyList& parameters, PropertyList& parametersEx, const PropertyList& calculated, const PropertyList& properties);
  void buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties);

  void buildPropertyView(IPropertyViewSourceObject* pSourceObject);

  Renga::IPropertyPtr getProperty(QtProperty* property);
  Renga::IOperationPtr createOperation();

  void resetUserAttribute(QtProperty* property);
  void changeUserAttribute(QtProperty* property, const double value);
  void changeUserAttribute(QtProperty* property, const std::wstring& value);

private:
  Renga::IApplicationPtr m_pApplication;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManagers m_propertyManagers;
  Mode m_propertyViewMode;

  std::unique_ptr<IPropertyViewSourceObject> m_pSourceObject;
};
