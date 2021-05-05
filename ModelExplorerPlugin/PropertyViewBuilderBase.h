//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "IPropertyViewBuilder.h"


class PropertyViewBuilderBase : public IPropertyViewBuilder
{
protected:
  PropertyViewBuilderBase(bool disableProperties = true);

  PropertyList createParametersInternal(PropertyManager& mng, Renga::IParameterContainer& container);
  PropertyList createPropertiesInternal(PropertyManager& mng, Renga::IPropertyContainer& container);
  PropertyList createQuantitiesInternal(PropertyManager& mng, Renga::IQuantityContainer& container);

  bool m_disableProperties = true;
};