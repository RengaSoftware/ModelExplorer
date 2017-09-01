//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "WallPropertyViewBuilder.h"

#include <RengaAPI/Wall.h>

WallPropertyViewBuilder::WallPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList WallPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Wall* pWall = dynamic_cast<rengaapi::Wall*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("wall", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("wall", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("wall", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pWall->name()));
  m_pPropertyManagers->m_pDoubleManager->setValue(offset, pWall->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pWall->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pWall->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList WallPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Wall* pWall = dynamic_cast<rengaapi::Wall*>(pObject);
	rengaapi::WallQuantities wallQuantities = pWall->quantities();
	
  QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "length"));
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "thickness"));
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "height"));
  QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "netVolume"));
	QtProperty* netFootprintArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "netFootprintArea"));
	QtProperty* netSideArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "netSideArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("wall", "mass"));

  PropertyList result;
	setLengthMeasureOptional(result, wallQuantities.nominalLength(), length);
  setLengthMeasureOptional(result, wallQuantities.nominalThickness(), thickness);
  setLengthMeasureOptional(result, wallQuantities.nominalHeight(), height);
	setVolumeMeasureOptional(result, wallQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, wallQuantities.netFootprintArea(), netFootprintArea);
	setAreaMeasureOptional(result, wallQuantities.netSideArea(), netSideArea);
	setMassMeasureOptional(result, wallQuantities.netMass(), mass);
  return result;
}