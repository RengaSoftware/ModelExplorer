//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "DoorPropertyViewBuilder.h"

#include <RengaAPI/Door.h>

DoorPropertyViewBuilder::DoorPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList DoorPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Door* pDoor = dynamic_cast<rengaapi::Door*>(pObject);
	PropertyList result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_door", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_door", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pDoor->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pDoor->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pDoor->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(mark);

  return result;
}


PropertyList DoorPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
  rengaapi::Door* pDoor = dynamic_cast<rengaapi::Door*>(pObject);
	rengaapi::DoorQuantities doorQuantities = pDoor->quantities();

	PropertyList result;

  // overallHeight, overallWidth, perimeter, area
	QtProperty* overallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "overallHeight"));
	QtProperty* overallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "overallWidth"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "perimeter"));
	QtProperty* area = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "area"));
	QtProperty* innerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "innerSurfaceArea"));
  QtProperty* innerSurfaceInternalArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "innerSurfaceInternalArea"));
  QtProperty* innerSurfaceExternalArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_door", "innerSurfaceExternalArea"));

	setLengthMeasureOptional(result, doorQuantities.overallHeight(), overallHeight);
	setLengthMeasureOptional(result, doorQuantities.overallWidth(), overallWidth);
	setLengthMeasureOptional(result, doorQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, doorQuantities.area(), area);
	setAreaMeasureOptional(result, doorQuantities.innerSurfaceArea(), innerSurfaceArea);
  setAreaMeasureOptional(result, doorQuantities.innerSurfaceInternalArea(), innerSurfaceInternalArea);
  setAreaMeasureOptional(result, doorQuantities.innerSurfaceExternalArea(), innerSurfaceExternalArea);

  return result;
}
