//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <qttreepropertybrowser.h>


typedef std::list<QtProperty*> PropertyList;

class IObjectPropertyViewBuilder
{
public:
  virtual void createParametersProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject) = 0;
  virtual void createQuantitiesProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject) = 0;
  virtual PropertyList createUserAttributesProperties(Renga::IModelObjectPtr pObject) = 0;
};
