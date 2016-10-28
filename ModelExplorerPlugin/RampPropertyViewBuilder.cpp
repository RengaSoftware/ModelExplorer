//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "RampPropertyViewBuilder.h"
#include "RengaStringToQString.h"

#include <RengaAPI/Ramp.h>

RampPropertyViewBuilder::RampPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap RampPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);
	PropertyMap result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("ramp", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("ramp", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("ramp", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pRamp->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pRamp->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pRamp->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pRamp->mark()));
	
	result.insert(std::make_pair(name->propertyName(), name));
  result.insert(std::make_pair(offset->propertyName(), offset));
	result.insert(std::make_pair(material->propertyName(), material));
	result.insert(std::make_pair(mark->propertyName(), mark));
  
	return result;
}


PropertyMap RampPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);
	PropertyMap result;
	rengaapi::RampQuantities rampQuantities = pRamp->quantities();

	//thickness, height, width, netVolume, netArea, netFloorArea, mass
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "height"));
  QtProperty* width = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "width"));
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "thickness"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "netVolume"));
	QtProperty* netArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "netArea"));
	QtProperty* netFloorArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "netFloorArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("ramp", "mass"));

  setLengthMeasureOptional(result, rampQuantities.nominalHeight(), height);
  setLengthMeasureOptional(result, rampQuantities.nominalWidth(), width);
  setLengthMeasureOptional(result, rampQuantities.nominalThickness(), thickness);
	setVolumeMeasureOptional(result, rampQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, rampQuantities.netArea(), netArea);
	setAreaMeasureOptional(result, rampQuantities.netFloorArea(), netFloorArea);
	setOneLayeredMass(result, pRamp->material(), rampQuantities.netVolume(), mass);

  return result;
}