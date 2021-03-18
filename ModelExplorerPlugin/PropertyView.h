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

  void showProperties(std::unique_ptr<IPropertyViewBuilder> builder,
                      Renga::IParameterContainerPtr parameters,
                      Renga::IPropertyContainerPtr properties);

  void changeMode(PropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

  void parameterBoolChanged(QtProperty* pProperty, bool val);
  void parameterIntChanged(QtProperty* pProperty, int val);
  void parameterDoubleChanged(QtProperty* pProperty, const QString& newValue);
  void parameterStringChanged(QtProperty* pProperty, const QString& newValue);

private:
  void updateParameters();
  void initPropertyManagers();
  void clearPropertyManagers();
  
  Renga::IPropertyPtr getProperty(QtProperty* property);
  Renga::IOperationPtr createOperation();

  void resetUserAttribute(QtProperty* property);
  void changeUserAttribute(QtProperty* property, const double value);
  void changeUserAttribute(QtProperty* property, const std::wstring& value);

private:
  Renga::IApplicationPtr m_pApplication;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManager m_commonAttributesMng;
  PropertyManager m_parametersMng;
  PropertyManager m_propertiesMng;
  Mode m_propertyViewMode;

  std::unique_ptr<IPropertyViewBuilder> m_builder;
  Renga::IPropertyContainerPtr m_properties;
  Renga::IParameterContainerPtr m_parameters;
};
