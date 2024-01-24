//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include "EntityPropertyViewBuilder.h"
#include "QtPropertiesConstruction.h"
#include "PropertyManager.h"


namespace
{
  void setPropertiesEnableFlag(const PropertyList& properties, bool enabled)
  {
    for (auto pProperty : properties)
      pProperty->setEnabled(enabled);
  }
}

EntityPropertyViewBuilder::EntityPropertyViewBuilder(
    ParameterContainerAccess parametersAccess,
    PropertyContainerAccess propertiesAccess,
    QuantityContainerAccess quantitiesAccess,
    EntityByParameterIdNameGetter entityNameGetter,
    Renga::IPropertyManagerPtr pRengaPropertyManager,
    bool disableProperties)
  : m_parametersAccess(parametersAccess),
    m_propertiesAccess(propertiesAccess),
    m_quantitiesAccess(quantitiesAccess),
    m_entityNameGetter(entityNameGetter),
    m_pRengaPropertyManager(pRengaPropertyManager),
    m_disableProperties(disableProperties)
{
}

void EntityPropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  // TODO: remove this condition, devide class free functions
  if (!m_parametersAccess)
    return;
  
  auto pParameters = m_parametersAccess();
  if (pParameters == nullptr)
    return;

  qtPropertiesFromRengaParameters(mngr, *pParameters, m_entityNameGetter);
}

void EntityPropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  // TODO: remove this condition, devide class free functions
  if (!m_quantitiesAccess)
    return;
  
  auto pQuantities = m_quantitiesAccess();
  if (pQuantities == nullptr)
    return;

  qtPropertiesFromRengaQuantities(mngr, *pQuantities);
}

void EntityPropertyViewBuilder::createProperties(PropertyManager& mngr)
{
  if (!m_propertiesAccess)
    return;

  auto pProperties = m_propertiesAccess();
  if (pProperties == nullptr)
    return;

  qtPropertiesFromRengaProperties(mngr, m_pRengaPropertyManager, pProperties);
  setPropertiesEnableFlag(mngr.properties(), !m_disableProperties);
}
