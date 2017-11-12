//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "OpeningPropertyViewBuilder.h"

#include <RengaAPI/Opening.h>

OpeningPropertyViewBuilder::OpeningPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList OpeningPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);
	PropertyList result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_opening", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_opening", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pOpening->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pOpening->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pOpening->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(mark);

  return result;
}


PropertyList OpeningPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);
	rengaapi::OpeningQuantities openingQuantities = pOpening->quantities();

  PropertyList result;

	// thickness, volume, perimeter, area
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "thickness"));
	QtProperty* volume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "volume"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "perimeter"));
	QtProperty* area = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "area"));
  QtProperty* innerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_opening", "innerSurfaceArea"));

  setLengthMeasureOptional(result, openingQuantities.nominalThickness(), thickness);
	setVolumeMeasureOptional(result, openingQuantities.volume(), volume);
	setLengthMeasureOptional(result, openingQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, openingQuantities.area(), area);
  setAreaMeasureOptional(result, openingQuantities.innerSurfaceArea(), innerSurfaceArea);

  return result;
}