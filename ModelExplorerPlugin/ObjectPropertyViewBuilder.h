//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "IObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

#include <qteditorfactory.h>


class ObjectPropertyViewBuilder : public IObjectPropertyViewBuilder
{
public:
  ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers, Renga::IApplicationPtr pApplication);

  virtual void createParametersProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject);
  virtual void createQuantitiesProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject);

  PropertyList createUserAttributesProperties(Renga::IModelObjectPtr pObject);

  QString getMaterialName(const int& materialId);
  QString getLayeredMaterialName(const int& layeredMaterialId);

  void addValue(PropertyList& propertyList, const QString& name, const int value) const;
  void addValue(PropertyList& propertyList, const QString& name, const double value) const;
  void addValue(PropertyList& propertyList, const QString& name, const QString& value) const;
  void addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityContainerPtr pQuantityContainer, const GUID quantityId) const;
  void addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityPtr pQuantity) const;

  //void pushValue(PropertyList& propertyList, QtProperty* pProperty);

protected:
  Renga::IApplicationPtr m_pApplication;
  const PropertyManagers* m_pPropertyManagers;
};
