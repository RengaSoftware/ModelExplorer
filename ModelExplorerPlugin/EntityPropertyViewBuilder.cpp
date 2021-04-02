//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include "EntityPropertyViewBuilder.h"


EntityPropertyViewBuilder::EntityPropertyViewBuilder(
    ParameterContainerAccess parametersAccess,
    PropertyContainerAccess propertiesAccess,
    QuantityContainerAccess quantitiesAccess,
    bool disableProperties)
  : PropertyViewBuilderBase(disableProperties),
    m_parametersAccess(parametersAccess),
    m_propertiesAccess(propertiesAccess),
    m_quantitiesAccess(quantitiesAccess)
{
}

void EntityPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  // TODO: remove this condition, devide class free functions
  auto pParameters = m_parametersAccess();
  if (pParameters == nullptr)
    return;

  auto properties = createParametersInternal(mngr, *pParameters);
  propertyList.splice(propertyList.end(), properties);
}

void EntityPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  // TODO: remove this condition, devide class free functions
  auto pQuantities = m_quantitiesAccess();
  if (pQuantities == nullptr)
    return;

  auto properties = createQuantitiesInternal(mngr, *pQuantities);
  propertyList.splice(propertyList.end(), properties);
}

PropertyList EntityPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  auto pProperties = m_propertiesAccess();
  if (pProperties == nullptr)
    return PropertyList{};

  return createPropertiesInternal(mngr, pProperties);
}
