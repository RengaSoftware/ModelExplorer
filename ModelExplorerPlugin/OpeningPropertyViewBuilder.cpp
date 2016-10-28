//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "OpeningPropertyViewBuilder.h"
#include "RengaStringToQString.h"

#include <RengaAPI/Opening.h>

OpeningPropertyViewBuilder::OpeningPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap OpeningPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);
	PropertyMap result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("opening", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("opening", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("opening", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pOpening->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pOpening->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pOpening->mark()));

	result.insert(std::make_pair(name->propertyName(), name));
	result.insert(std::make_pair(offset->propertyName(), offset));
	result.insert(std::make_pair(mark->propertyName(), mark));

  return result;
}


PropertyMap OpeningPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);
	rengaapi::OpeningQuantities openingQuantities = pOpening->quantities();

  PropertyMap result;

	// thickness, volume, perimeter, area
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("opening", "thickness"));
	QtProperty* volume = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("opening", "volume"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("opening", "perimeter"));
	QtProperty* area = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("opening", "area"));

  setLengthMeasureOptional(result, openingQuantities.nominalThickness(), thickness);
	setVolumeMeasureOptional(result, openingQuantities.volume(), volume);
	setLengthMeasureOptional(result, openingQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, openingQuantities.area(), area);

  return result;
}