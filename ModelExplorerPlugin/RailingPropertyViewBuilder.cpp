//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RailingPropertyViewBuilder.h"

#include <RengaAPI/Railing.h>

RailingPropertyViewBuilder::RailingPropertyViewBuilder(const PropertyManagers* pPropertyManagers) 
  : ObjectPropertyViewBuilder(pPropertyManagers)
{}

PropertyList RailingPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);
	PropertyList result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("railing", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("railing", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(QApplication::translate("railing", "mark"));

  m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pRailing->name()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pRailing->mark()));

	result.push_back(name);
	setLengthMeasureOptional(result, pRailing->offset(), offset);
	result.push_back(mark);

  return result;
}


PropertyList RailingPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);
	rengaapi::RailingQuantities railingQuantities = pRailing->quantities();

	// height, length
  PropertyList result;
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("railing", "height"));
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(QApplication::translate("railing", "length"));

  setLengthMeasureOptional(result, railingQuantities.nominalHeight(), height);
	setLengthMeasureOptional(result, railingQuantities.nominalLength(), length);

  return result;
}