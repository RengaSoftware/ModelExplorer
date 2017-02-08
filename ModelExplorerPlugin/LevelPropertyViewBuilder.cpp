//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "LevelPropertyViewBuilder.h"

#include <RengaAPI/Level.h>

LevelPropertyViewBuilder::LevelPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyList LevelPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Level* pLevel = dynamic_cast<rengaapi::Level*>(pObject);
	PropertyList result;

	// name, elevation
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("level", "name"));
	QtProperty* elevation = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("level", "elevation"));

  m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pLevel->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(elevation, pLevel->elevation().inMeters());
	
	result.push_back(name);
	result.push_back(elevation);

  return result;
}

PropertyList LevelPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject*)
{
  return PropertyList();
}