//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FloorPropertyViewBuilder.h"

#include <RengaAPI/Floor.h>

FloorPropertyViewBuilder::FloorPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList FloorPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Floor* pFloor = dynamic_cast<rengaapi::Floor*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("floor", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("floor", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("floor", "mark"));

  m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pFloor->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pFloor->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pFloor->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pFloor->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList FloorPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Floor* pFloor = dynamic_cast<rengaapi::Floor*>(pObject);
  rengaapi::FloorQuantities floorQuantities = pFloor->quantities();

  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "thickness"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "netVolume"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "perimeter"));
	QtProperty* netArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "netArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("floor", "mass"));

  PropertyList result;
	setLengthMeasureOptional(result, floorQuantities.nominalThickness(), thickness);
  setVolumeMeasureOptional(result, floorQuantities.netVolume(), netVolume);
	setLengthMeasureOptional(result, floorQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, floorQuantities.netArea(), netArea);
	setMassMeasureOptional(result, floorQuantities.netMass(), mass);
  return result;
}