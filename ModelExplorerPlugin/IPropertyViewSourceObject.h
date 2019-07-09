#pragma once

#include "IPropertyViewBuilder.h"

class PropertyManagers;

class IPropertyViewSourceObject
{
public:
  virtual ~IPropertyViewSourceObject() {}

  virtual std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder(PropertyManagers* pPropertyManagers) = 0;

  virtual Renga::IParameterPtr getParameter(GUID parameterId) = 0;
  virtual Renga::IPropertyPtr getUserDefinedProperty(GUID propertyId) = 0;
};

class CPropertyViewSourceObject : public IPropertyViewSourceObject
{
public:
  Renga::IParameterPtr getParameter(GUID parameterId) override { return nullptr; }
  Renga::IPropertyPtr getUserDefinedProperty(GUID propertyId) override { return nullptr; }
};
