#pragma once

#include "IPropertyViewBuilder.h"

class PropertyManagers;

class IPropertyViewSourceObject
{
public:
  virtual ~IPropertyViewSourceObject() {}

  virtual std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder() = 0;
};
