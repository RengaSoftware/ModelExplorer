//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "ColumnPropertyViewBuilder.h"
#include "RengaStringToQString.h"

#include <RengaAPI/Column.h>

ColumnPropertyViewBuilder::ColumnPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap ColumnPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);
	PropertyMap result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("column", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("column", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("column", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pColumn->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pColumn->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pColumn->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pColumn->mark()));

	result.insert(std::make_pair(name->propertyName(), name));
	result.insert(std::make_pair(offset->propertyName(), offset));
	result.insert(std::make_pair(material->propertyName(), material));
	result.insert(std::make_pair(mark->propertyName(), mark));

  return result;
}


PropertyMap ColumnPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);
	PropertyMap result;
	rengaapi::ColumnQuantities columnQuantities = pColumn->quantities();

	// length, netVolume, crossSectionArea, outerSurfaceArea, perimeter, mass
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "length"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "netVolume"));
	QtProperty* crossSectionArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "crossSectionArea"));
	QtProperty* outerSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "outerSurfaceArea"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "perimeter"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("column", "mass"));

	setLengthMeasureOptional(result, columnQuantities.nominalLength(), length);
	setVolumeMeasureOptional(result, columnQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, columnQuantities.crossSectionArea(), crossSectionArea);
	setAreaMeasureOptional(result, columnQuantities.outerSurfaceArea(), outerSurfaceArea);
	setLengthMeasureOptional(result, columnQuantities.perimeter(), perimeter);
	setOneLayeredMass(result, pColumn->material(), columnQuantities.netVolume(), mass);

  return result;
}