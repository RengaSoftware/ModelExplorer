//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "RailingPropertyViewBuilder.h"
#include "RengaStringToQString.h"

#include <RengaAPI/Railing.h>

RailingPropertyViewBuilder::RailingPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator) 
  : ObjectPropertyViewBuilder(pPropertyManagers, pTranslator)
{}

PropertyMap RailingPropertyViewBuilder::createParametersProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);
	PropertyMap result;

	// name, offset, mark
	QtProperty* name = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("railing", "name"));
	QtProperty* offset = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("railing", "offset"));
	QtProperty* mark = m_pPropertyManagers->m_pStringManager->addProperty(m_pTranslator->translate("railing", "mark"));

  m_pPropertyManagers->m_pStringManager->setValue(name, rengaStringToQString(pRailing->name()));
	m_pPropertyManagers->m_pStringManager->setValue(mark, rengaStringToQString(pRailing->mark()));

	result.insert(std::make_pair(name->propertyName(), name));
	setLengthMeasureOptional(result, pRailing->offset(), offset);
	result.insert(std::make_pair(mark->propertyName(), mark));

  return result;
}


PropertyMap RailingPropertyViewBuilder::createQuantitiesProperties(rengaapi::ModelObject* pObject)
{
	rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);
	rengaapi::RailingQuantities railingQuantities = pRailing->quantities();

	// height, length
  PropertyMap result;
  QtProperty* height = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("railing", "height"));
	QtProperty* length = m_pPropertyManagers->m_pDoubleManager->addProperty(m_pTranslator->translate("railing", "length"));

  setLengthMeasureOptional(result, railingQuantities.nominalHeight(), height);
	setLengthMeasureOptional(result, railingQuantities.nominalLength(), length);

  return result;
}