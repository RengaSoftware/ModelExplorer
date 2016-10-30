//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "WindowPropertyViewBuilder.h"

#include <RengaAPI/Window.h>

WindowPropertyViewBuilder::WindowPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap WindowPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);
	PropertyMap result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("window", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("window", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("window", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pWindow->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pWindow->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pWindow->mark()));

	result.insert(std::make_pair(name->propertyName(), name));
	result.insert(std::make_pair(offset->propertyName(), offset));
	result.insert(std::make_pair(mark->propertyName(), mark));

  return result;
}


PropertyMap WindowPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);
	rengaapi::WindowQuantities windowQuantities = pWindow->quantities();

  PropertyMap result;

	// overallHeight, overallWidth, perimeter, area
	QtProperty* overallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("window", "overallHeight"));
	QtProperty* overallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("window", "overallWidth"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("window", "perimeter"));
	QtProperty* area = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("window", "area"));

	setLengthMeasureOptional(result, windowQuantities.overallHeight(), overallHeight);
	setLengthMeasureOptional(result, windowQuantities.overallWidth(), overallWidth);
	setLengthMeasureOptional(result, windowQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, windowQuantities.area(), area);

  return result;
}