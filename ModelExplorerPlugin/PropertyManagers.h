//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>

#include "IPropertyViewBuilder.h"


class PropertyManagers
{
public:
  PropertyManagers();

  QtProperty* addValue(const QString& name, const double value) const;
  QtProperty* addValue(const QString& name, const QString& value) const;

  void addValue(PropertyList& propertyList, const QString& name, const int value) const;
  void addValue(PropertyList& propertyList, const QString& name, const double value) const;
  void addValue(PropertyList& propertyList, const QString& name, const QString& value) const;
  void addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityContainerPtr pQuantityContainer, const GUID quantityId) const;
  void addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityPtr pQuantity) const;

public:
  QtIntPropertyManager* m_pIntManager;
  QtStringPropertyManager* m_pDoubleManager;
  QtStringPropertyManager* m_pStringManager;
  QtStringPropertyManager* m_pDoubleUserAttributeManager;
  QtStringPropertyManager* m_pStringUserAttributeManager;
};