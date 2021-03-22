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
    Renga::IParameterContainerPtr pParameters,
    Renga::IPropertyContainerPtr pProperties,
    Renga::IQuantityContainerPtr pQuantities)
  : m_pParameters(pParameters),
    m_pProperties(pProperties),
    m_pQuantities(pQuantities)
{
}

void EntityPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  // TODO: remove this condition, devide class free functions
  if (m_pParameters == nullptr)
    return;

  auto properties = createParametersInternal(mngr, *m_pParameters);
  propertyList.splice(propertyList.end(), properties);
}

void EntityPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  // TODO: remove this condition, devide class free functions
  if (m_pQuantities == nullptr)
    return;

  auto properties = createQuantitiesInternal(mngr, *m_pQuantities);
  propertyList.splice(propertyList.end(), properties);
}

PropertyList EntityPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  return createPropertiesInternal(mngr, m_pProperties);
}
