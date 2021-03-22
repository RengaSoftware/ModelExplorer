//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include "ObjectPropertyViewBuilder.h"


ObjectPropertyViewBuilder::ObjectPropertyViewBuilder(
    Renga::IParameterContainerPtr pParameters,
    Renga::IPropertyContainerPtr pProperties,
    Renga::IQuantityContainerPtr pQuantities)
  : m_pParameters(pParameters),
    m_pProperties(pProperties),
    m_pQuantities(pQuantities)
{
}

void ObjectPropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  auto properties = createParametersInternal(mngr, *m_pParameters);
  propertyList.splice(propertyList.end(), properties);
}

void ObjectPropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  auto properties = createQuantitiesInternal(mngr, *m_pQuantities);
  propertyList.splice(propertyList.end(), properties);
}

PropertyList ObjectPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  return createPropertiesInternal(mngr, m_pProperties);
}
