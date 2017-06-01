//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "StairPropertyViewBuilder.h"

#include <RengaAPI/Stair.h>

StairPropertyViewBuilder::StairPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList StairPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Stair* pStair = dynamic_cast<rengaapi::Stair*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("stair", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("stair", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("stair", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pStair->name()));
  m_pPropertyManagers->m_pDoubleManager->setValue(offset, pStair->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pStair->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pStair->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList StairPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Stair* pStair = dynamic_cast<rengaapi::Stair*>(pObject);
	PropertyList result;
	rengaapi::StairQuantities stairQuantities = pStair->quantities();

	// height, width, netVolume, numberOfRiser, numberOfTreads, riserHeight, treadLength, netArea, netFloorArea, mass
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "height"));
  QtProperty* width = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "width"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "netVolume"));
	QtProperty* numberOfRiser = m_pPropertyManagers->m_pIntManager->addProperty(QApplication::translate("stair", "numberOfRiser"));
	QtProperty* numberOfTreads = m_pPropertyManagers->m_pIntManager->addProperty(QApplication::translate("stair", "numberOfTreads"));
	QtProperty* riserHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "riserHeight"));
	QtProperty* treadLength = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "treadLength"));
	QtProperty* netArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "netArea"));
	QtProperty* netFloorArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "netFloorArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("stair", "mass"));

	m_pPropertyManagers->m_pIntManager->setValue(numberOfRiser, stairQuantities.numberOfRiser());
	m_pPropertyManagers->m_pIntManager->setValue(numberOfTreads, stairQuantities.numberOfTreads());

  setLengthMeasureOptional(result, stairQuantities.nominalHeight(), height);
  setLengthMeasureOptional(result, stairQuantities.nominalWidth(), width);
	setVolumeMeasureOptional(result, stairQuantities.netVolume(), netVolume);
	result.push_back(numberOfRiser);
	result.push_back(numberOfTreads);
	setLengthMeasureOptional(result, stairQuantities.riserHeight(), riserHeight);
	setLengthMeasureOptional(result, stairQuantities.treadLength(), treadLength);
	setAreaMeasureOptional(result, stairQuantities.netArea(), netArea);
	setAreaMeasureOptional(result, stairQuantities.netFloorArea(), netFloorArea);
	setOneLayeredMass(result, pStair->material(), stairQuantities.netVolume(), mass);
	
  return result;
}