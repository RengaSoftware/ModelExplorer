//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "PropertyViewBuilderBase.h"
#include "RengaModelUtils.h"


class EntityPropertyViewBuilder : public PropertyViewBuilderBase
{
public:
  EntityPropertyViewBuilder(
      ParameterContainerAccess parametersAccess,
      PropertyContainerAccess propertiesAccess,
      QuantityContainerAccess quantitiesAccess,
      bool disableProperties);

  // IPropertyViewBuilder
  void createParameters(PropertyManager& mng) override;
  void createQuantities(PropertyManager& mng) override;
  void createProperties(PropertyManager& mng) override;

protected:
  ParameterContainerAccess m_parametersAccess;
  PropertyContainerAccess m_propertiesAccess;
  QuantityContainerAccess m_quantitiesAccess;
};
