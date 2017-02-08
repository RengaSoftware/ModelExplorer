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

FloorPropertyViewBuilder::FloorPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyList FloorPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Floor* pFloor = dynamic_cast<rengaapi::Floor*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("floor", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("floor", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("floor", "mark"));

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
	PropertyList result;

	rengaapi::FloorQuantities floorQuantities = pFloor->quantities();
	rengaapi::FloorMaterialLayerQuantityCollection floorMaterialLayerQuantityCollection = floorQuantities.materialQuantities();
	std::vector<rengabase::VolumeMeasureOptional> volumeCollection;
	for (size_t i = 0 ; i < floorMaterialLayerQuantityCollection.size() ; ++i)
  {
		volumeCollection.push_back(floorMaterialLayerQuantityCollection.get(i).netVolume());
  }

	// nominalThickness, netVolume, perimeter, netArea, mass
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "thickness"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "netVolume"));
	QtProperty* perimeter = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "perimeter"));
	QtProperty* netArea = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "netArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("floor", "mass"));

	setLengthMeasureOptional(result, floorQuantities.nominalThickness(), thickness);
  setVolumeMeasureOptional(result, floorQuantities.netVolume(), netVolume);
	setLengthMeasureOptional(result, floorQuantities.perimeter(), perimeter);
	setAreaMeasureOptional(result, floorQuantities.netArea(), netArea);
	setMultiLayeredMass(result, pFloor->material(), volumeCollection, mass);

  return result;
}