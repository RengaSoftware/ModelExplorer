#pragma once

#include "IPropertyViewBuilder.h"

class PropertyManagers;

class IPropertyViewSourceObject
{
public:
  virtual ~IPropertyViewSourceObject() {}

  virtual std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder(PropertyManagers* pPropertyManagers) = 0;
  virtual Renga::IParameterPtr getParameter(GUID parameterId) = 0;
};
