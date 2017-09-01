//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RampPropertyViewBuilder.h"

#include <RengaAPI/Ramp.h>

RampPropertyViewBuilder::RampPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList RampPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("ramp", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("ramp", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("ramp", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pRamp->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pRamp->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pRamp->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pRamp->mark()));
	
	result.push_back(name);
  result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);
  
	return result;
}


PropertyList RampPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);
	PropertyList result;
	rengaapi::RampQuantities rampQuantities = pRamp->quantities();

	//thickness, height, width, netVolume, netArea, netFloorArea, mass
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "height"));
  QtProperty* width = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "width"));
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "thickness"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "netVolume"));
	QtProperty* netArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "netArea"));
	QtProperty* netFloorArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "netFloorArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("ramp", "mass"));

  setLengthMeasureOptional(result, rampQuantities.nominalHeight(), height);
  setLengthMeasureOptional(result, rampQuantities.nominalWidth(), width);
  setLengthMeasureOptional(result, rampQuantities.nominalThickness(), thickness);
	setVolumeMeasureOptional(result, rampQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, rampQuantities.netArea(), netArea);
	setAreaMeasureOptional(result, rampQuantities.netFloorArea(), netFloorArea);
	setMassMeasureOptional(result, rampQuantities.netMass(), mass);

  return result;
}