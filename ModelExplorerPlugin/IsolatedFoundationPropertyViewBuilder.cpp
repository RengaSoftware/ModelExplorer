//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "IsolatedFoundationPropertyViewBuilder.h"

#include <RengaAPI/IsolatedFoundation.h>

IsolatedFoundationPropertyViewBuilder::IsolatedFoundationPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList IsolatedFoundationPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::IsolatedFoundation* pIsolatedFoundation = dynamic_cast<rengaapi::IsolatedFoundation*>(pObject);
	PropertyList result;

	// name, offset, material, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_isolatedFoundation", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "offset"));
	QtProperty* material = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_isolatedFoundation", "material"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("me_isolatedFoundation", "mark"));

	m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pIsolatedFoundation->name()));
	m_pPropertyManagers->m_pDoubleManager->setValue(offset, pIsolatedFoundation->offset().inMillimeters());
	m_pPropertyManagers->m_pStringManager->setValue(material, getMaterialName(pIsolatedFoundation->material()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pIsolatedFoundation->mark()));

	result.push_back(name);
	result.push_back(offset);
	result.push_back(material);
	result.push_back(mark);

  return result;
}


PropertyList IsolatedFoundationPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::IsolatedFoundation* pIsolatedFoundation = dynamic_cast<rengaapi::IsolatedFoundation*>(pObject);
	PropertyList result;
	rengaapi::IsolatedFoundationQuantities isolatedFoundationQuantities = pIsolatedFoundation->quantities();

	// overallWidth, overallDepth, overallHeight, netFootprintArea, netVolume, mass
	QtProperty* overallWidth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "overallWidth"));
	QtProperty* overallDepth = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "overallDepth"));
	QtProperty* overallHeight = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "overallHeight"));
	QtProperty* netFootprintArea = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "netFootprintArea"));
	QtProperty* netVolume = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "netVolume"));
	QtProperty* mass = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("me_isolatedFoundation", "mass"));

	setLengthMeasureOptional(result, isolatedFoundationQuantities.overallWidth(), overallWidth);
	setLengthMeasureOptional(result, isolatedFoundationQuantities.overallDepth(), overallDepth);
	setLengthMeasureOptional(result, isolatedFoundationQuantities.overallHeight(), overallHeight);
	setAreaMeasureOptional(result, isolatedFoundationQuantities.netFootprintArea(), netFootprintArea);
	setVolumeMeasureOptional(result, isolatedFoundationQuantities.netVolume(), netVolume);
	setMassMeasureOptional(result, isolatedFoundationQuantities.netMass(), mass);

  return result;
}