//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once


class PropertyManager;

class IPropertyViewBuilder
{
public:
  virtual ~IPropertyViewBuilder() {}

  // TODO: return pure virtual
  virtual void createParameters(PropertyManager& mng) = 0;
  virtual void createQuantities(PropertyManager& mng) = 0;
  virtual void createProperties(PropertyManager& mng) = 0;
};
