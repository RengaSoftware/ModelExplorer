//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "BeamPropertyViewBuilder.h"

#include <RengaAPI/Beam.h>

BeamPropertyViewBuilder::BeamPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList BeamPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Beam* pBeam = dynamic_cast<rengaapi::Beam*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_beam", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_beam", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_beam", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pBeam->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pBeam->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pBeam->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pBeam->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList BeamPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Beam* pBeam = dynamic_cast<rengaapi::Beam*>(pObject);
	PropertyList result;
	rengaapi::BeamQuantities beamQuantities = pBeam->quantities();

	// length, netVolume, crossSectionArea, outerSurfaceArea, perimeter, mass
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "length"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "netVolume"));
  QtProperty* crossSectionOverallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "crossSectionOverallWidth"));
  QtProperty* crossSectionOverallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "crossSectionOverallHeight"));
  QtProperty* crossSectionArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "crossSectionArea"));
	QtProperty* outerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "outerSurfaceArea"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "perimeter"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_beam", "mass"));

	setLengthMeasureOptional(result, beamQuantities.nominalLength(), length);
	setVolumeMeasureOptional(result, beamQuantities.netVolume(), netVolume);
  setLengthMeasureOptional(result, beamQuantities.crossSectionOverallWidth(), crossSectionOverallWidth);
  setLengthMeasureOptional(result, beamQuantities.crossSectionOverallHeight(), crossSectionOverallHeight);
  setAreaMeasureOptional(result, beamQuantities.crossSectionArea(), crossSectionArea, MeasureUnit::Centimeter);
	setAreaMeasureOptional(result, beamQuantities.outerSurfaceArea(), outerSurfaceArea);
	setLengthMeasureOptional(result, beamQuantities.perimeter(), perimeter);
	setMassMeasureOptional(result, beamQuantities.netMass(), mass);

  return result;
}