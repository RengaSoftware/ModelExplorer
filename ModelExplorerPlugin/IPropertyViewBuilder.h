//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "PropertyList.h"


class IPropertyViewBuilder
{
public:
  virtual ~IPropertyViewBuilder() {}

  virtual void createIntegratedParameters(PropertyList& propertyList) = 0;
  virtual void createParameters(PropertyList& propertyList) = 0;
  virtual void createQuantities(PropertyList& propertyList) = 0;
  virtual PropertyList createProperties() = 0;
};

class CPropertyViewBuilder : public IPropertyViewBuilder
{
public:
  void createParameters(PropertyList& propertyList) override {}
  PropertyList createProperties() override { return PropertyList(); }
};
