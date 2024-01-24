//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class EntityPropertyViewBuilder : public IPropertyViewBuilder
{
public:
  EntityPropertyViewBuilder(
      ParameterContainerAccess parametersAccess,
      PropertyContainerAccess propertiesAccess,
      QuantityContainerAccess quantitiesAccess,
      EntityByParameterIdNameGetter entityNameGetter,
      Renga::IPropertyManagerPtr pRengaPropertyManager,
      bool disableProperties);

  // IPropertyViewBuilder
  void createParameters(PropertyManager& mng) override;
  void createQuantities(PropertyManager& mng) override;
  void createProperties(PropertyManager& mng) override;

protected:
  ParameterContainerAccess m_parametersAccess;
  PropertyContainerAccess m_propertiesAccess;
  QuantityContainerAccess m_quantitiesAccess;
  EntityByParameterIdNameGetter m_entityNameGetter;
  Renga::IPropertyManagerPtr m_pRengaPropertyManager;
  bool m_disableProperties;
};
