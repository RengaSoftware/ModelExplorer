//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoofPropertyViewBuilder.h"

#include <RengaAPI/Roof.h>

RoofPropertyViewBuilder::RoofPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList RoofPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Roof* pRoof = dynamic_cast<rengaapi::Roof*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("roof", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("roof", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("roof", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("roof", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pRoof->name()));

  m_pPropertyManagers->m_pDoubleManager->setValue(offset, pRoof->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pRoof->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pRoof->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList RoofPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Roof* pRoof = dynamic_cast<rengaapi::Roof*>(pObject);
	PropertyList result;

	rengaapi::RoofQuantities roofQuantities = pRoof->quantities();
	rengaapi::RoofMaterialLayerQuantityCollection roofMaterialLayerQuantityCollection = roofQuantities.materialQuantities();
	std::vector<rengabase::VolumeMeasureOptional> volumeCollection;
	for (size_t i = 0 ; i < roofMaterialLayerQuantityCollection.size() ; ++i)
  {
		volumeCollection.push_back(roofMaterialLayerQuantityCollection.get(i).netVolume());
  }

	// thickness, netVolume, totalSurfaceArea, mass
  QtProperty* thickness = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("roof", "thickness"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("roof", "netVolume"));
	QtProperty* totalSurfaceArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("roof", "totalSurfaceArea"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("roof", "mass"));

  setLengthMeasureOptional(result, roofQuantities.nominalThickness(), thickness);
	setVolumeMeasureOptional(result, roofQuantities.netVolume(), netVolume);
	setAreaMeasureOptional(result, roofQuantities.totalSurfaceArea(), totalSurfaceArea);
	setMultiLayeredMass(result, pRoof->material(), volumeCollection, mass);

  return result;
}