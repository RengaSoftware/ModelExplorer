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
  void setSelectedObjectId(int objId);
  void changeMode(ObjectPropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  bool createProperties(PropertyList& parameters, PropertyList& calculated, PropertyList& properties);
  void buildPropertyViewAsList(PropertyList& parameters, PropertyList& calculated, PropertyList& properties);
  void buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& properties);
  void buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties);
  void buildPropertyView();


  Renga::IPropertyPtr getProperty(QtProperty* property);
  Renga::IOperationPtr createOperation();

  void resetUserAttribute(QtProperty* property);
  void changeUserAttribute(QtProperty* property, const double value);
  void changeUserAttribute(QtProperty* property, const std::wstring& value);

private:
  Renga::IApplicationPtr m_pApplication;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManagers m_propertyManagers;
  int m_currentObjectId;
  Mode m_propertyViewMode;
};
