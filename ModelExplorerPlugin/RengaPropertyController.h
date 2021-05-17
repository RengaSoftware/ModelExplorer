//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "RengaModelUtils.h"


class QtProperty;

class RengaPropertyController : public QObject
{
  Q_OBJECT
public:
  RengaPropertyController(Renga::IApplicationPtr pRenga, PropertyContainerAccess propertiesAccess);

public slots:
  void onDoublePropertyChanged(QtProperty* property, const QString& newValue);
  void onStringPropertyChanged(QtProperty* property, const QString& newValue);
  void onIntPropertyChanged(QtProperty* property, int value);
  void onBoolPropertyChanged(QtProperty* property, bool value);

private:
  Renga::IPropertyPtr getProperty(QtProperty* property);
  Renga::IOperationPtr createOperation();
  bool tryGetEnumValueByIndex(GUID propertyId, int index, QString& result);
  
  void resetPropertyValue(QtProperty* property);
  void changePropertyValue(QtProperty* property, const double value);
  void changePropertyValue(QtProperty* property, const QString& value);
  void changePropertyValue(QtProperty* property, int value);
  void changePropertyValue(QtProperty* property, bool value);

private:
  Renga::IApplicationPtr m_pRenga;
  PropertyContainerAccess m_propertiesAccess;
};
