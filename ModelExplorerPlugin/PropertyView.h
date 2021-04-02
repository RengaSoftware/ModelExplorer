//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyManager.h"
#include "IPropertyViewBuilder.h"
#include "IPropertyViewSourceObject.h"

#include <qttreepropertybrowser.h>


using PropertyContainerAccess = std::function<Renga::IPropertyContainerPtr()>;

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

  void showProperties(std::unique_ptr<IPropertyViewBuilder> builder, PropertyContainerAccess propertiesAccess);
  void changeMode(PropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

private:
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
  PropertyManager m_quantitiesMng;
  PropertyManager m_parametersMng;
  PropertyManager m_propertiesMng;
  Mode m_propertyViewMode;

  std::unique_ptr<IPropertyViewBuilder> m_builder;
  PropertyContainerAccess m_propertiesAccess;
};
