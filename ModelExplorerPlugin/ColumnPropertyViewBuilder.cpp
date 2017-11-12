//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ColumnPropertyViewBuilder.h"

#include <RengaAPI/Column.h>

ColumnPropertyViewBuilder::ColumnPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList ColumnPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_column", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_column", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_column", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pColumn->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pColumn->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pColumn->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pColumn->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList ColumnPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);
	PropertyList result;
	rengaapi::ColumnQuantities columnQuantities = pColumn->quantities();

	// length, netVolume, crossSectionArea, outerSurfaceArea, perimeter, mass
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "length"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "netVolume"));
  QtProperty* crossSectionOverallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "crossSectionOverallWidth"));
  QtProperty* crossSectionOverallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "crossSectionOverallHeight"));
  QtProperty* crossSectionArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "crossSectionArea"));
	QtProperty* outerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "outerSurfaceArea"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "perimeter"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_column", "mass"));

	setLengthMeasureOptional(result, columnQuantities.nominalLength(), length);
	setVolumeMeasureOptional(result, columnQuantities.netVolume(), netVolume);
  setLengthMeasureOptional(result, columnQuantities.crossSectionOverallWidth(), crossSectionOverallWidth);
  setLengthMeasureOptional(result, columnQuantities.crossSectionOverallHeight(), crossSectionOverallHeight);
  setAreaMeasureOptional(result, columnQuantities.crossSectionArea(), crossSectionArea, MeasureUnit::Centimeter);
	setAreaMeasureOptional(result, columnQuantities.outerSurfaceArea(), outerSurfaceArea);
	setLengthMeasureOptional(result, columnQuantities.perimeter(), perimeter);
	setMassMeasureOptional(result, columnQuantities.netMass(), mass);

  return result;
}