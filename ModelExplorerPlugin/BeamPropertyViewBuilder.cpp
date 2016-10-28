//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "BeamPropertyViewBuilder.h"
#include "RengaStringToQString.h"

#include <RengaAPI/Beam.h>

BeamPropertyViewBuilder::BeamPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap BeamPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Beam* pBeam = dynamic_cast<rengaapi::Beam*>(pObject);
	PropertyMap result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("beam", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("beam", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("beam", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pBeam->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pBeam->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pBeam->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pBeam->mark()));

	result.insert(std::make_pair(name->propertyName(), name));
	result.insert(std::make_pair(offset->propertyName(), offset));
	result.insert(std::make_pair(material->propertyName(), material));
	result.insert(std::make_pair(mark->propertyName(), mark));

  return result;
}


PropertyMap BeamPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Beam* pBeam = dynamic_cast<rengaapi::Beam*>(pObject);
	PropertyMap result;
	rengaapi::BeamQuantities beamQuantities = pBeam->quantities();

	// length, netVolume, crossSectionArea, outerSurfaceArea, perimeter, mass
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "length"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "netVolume"));
	QtProperty* crossSectionArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "crossSectionArea"));
	QtProperty* outerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "outerSurfaceArea"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "perimeter"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("beam", "mass"));

	setLengthMeasureOptional(result, beamQuantities.nominalLength(), length);
	setVolumeMeasureOptional(result, beamQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, beamQuantities.crossSectionArea(), crossSectionArea);
	setAreaMeasureOptional(result, beamQuantities.outerSurfaceArea(), outerSurfaceArea);
	setLengthMeasureOptional(result, beamQuantities.perimeter(), perimeter);
	setOneLayeredMass(result, pBeam->material(), beamQuantities.netVolume(), mass);

  return result;
}