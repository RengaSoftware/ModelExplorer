//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "PropertyViewBuilderBase.h"


class EntityPropertyViewBuilder : public PropertyViewBuilderBase
{
public:
  EntityPropertyViewBuilder(
      Renga::IParameterContainerPtr pParameters,
      Renga::IPropertyContainerPtr pProperties,
      Renga::IQuantityContainerPtr pQuantities,
      bool disableProperties);

  // IPropertyViewBuilder
  void createParameters(PropertyManager& mng, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mng, PropertyList& propertyList) override;
  PropertyList createProperties(PropertyManager& mng) override;

protected:
  Renga::IParameterContainerPtr m_pParameters;
  Renga::IPropertyContainerPtr m_pProperties;
  Renga::IQuantityContainerPtr m_pQuantities;
};
