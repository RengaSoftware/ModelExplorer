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

PropertyList EntityPropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  // TODO: remove this condition, devide class free functions
  if (!m_parametersAccess)
    return {};
  
  auto pParameters = m_parametersAccess();
  if (pParameters == nullptr)
    return {};

  return createParametersInternal(mngr, *pParameters);
}

PropertyList EntityPropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  // TODO: remove this condition, devide class free functions
  if (!m_quantitiesAccess)
    return {};
  
  auto pQuantities = m_quantitiesAccess();
  if (pQuantities == nullptr)
    return {};

  return createQuantitiesInternal(mngr, *pQuantities);
}

PropertyList EntityPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  if (!m_propertiesAccess)
    return {};

  auto pProperties = m_propertiesAccess();
  if (pProperties == nullptr)
    return {};

  return createPropertiesInternal(mngr, pProperties);
}
