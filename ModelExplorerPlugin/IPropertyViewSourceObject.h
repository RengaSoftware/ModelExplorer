#pragma once

#include "IObjectPropertyViewBuilder.h"

class PropertyManagers;

class IPropertyViewSourceObject
{
public:
  virtual ~IPropertyViewSourceObject() {}

  virtual IObjectPropertyViewBuilder* createPropertyViewBuilder(PropertyManagers* pPropertyManagers) = 0;

  virtual Renga::IPropertyPtr getUserDefinedProperty(GUID propertyId) = 0;
};
