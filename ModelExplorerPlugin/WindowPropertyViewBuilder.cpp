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

WindowPropertyViewBuilder::WindowPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList WindowPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);
	PropertyList result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_window", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_window", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pWindow->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pWindow->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pWindow->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(mark);

  return result;
}


PropertyList WindowPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);
	rengaapi::WindowQuantities windowQuantities = pWindow->quantities();

  PropertyList result;

	// overallHeight, overallWidth, perimeter, area
	QtProperty* overallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "overallHeight"));
	QtProperty* overallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "overallWidth"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "perimeter"));
	QtProperty* area = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "area"));
	QtProperty* innerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "innerSurfaceArea"));
  QtProperty* innerSurfaceInternalArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "innerSurfaceInternalArea"));
  QtProperty* innerSurfaceExternalArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_window", "innerSurfaceExternalArea"));

	setLengthMeasureOptional(result, windowQuantities.overallHeight(), overallHeight);
	setLengthMeasureOptional(result, windowQuantities.overallWidth(), overallWidth);
	setLengthMeasureOptional(result, windowQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, windowQuantities.area(), area);
	setAreaMeasureOptional(result, windowQuantities.innerSurfaceArea(), innerSurfaceArea);
  setAreaMeasureOptional(result, windowQuantities.innerSurfaceInternalArea(), innerSurfaceInternalArea);
  setAreaMeasureOptional(result, windowQuantities.innerSurfaceExternalArea(), innerSurfaceExternalArea);

  return result;
}
