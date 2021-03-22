//
// Copyright �Renga Software� LLC, 2016. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "PropertyList.h"


class PropertyManager;

// TODO: this interface is used to create properties from some Renga entity.
// The source object now contained in concrete implementations, but better to use common abstract containers
// in future (Renga::IPropertyContainer, IParameterContainer, IQuantityContainer)
// Also better to return PropertyList as a value

class IPropertyViewBuilder
{
public:
  virtual ~IPropertyViewBuilder() {}

  // TODO: return pure virtual
  virtual void createParameters(PropertyManager& mng, PropertyList& propertyList) {};
  virtual void createQuantities(PropertyManager& mng, PropertyList& propertyList) {};
  virtual PropertyList createProperties(PropertyManager& mng) { return PropertyList{}; };
};
