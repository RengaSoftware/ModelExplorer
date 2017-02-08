//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyViewBuilder.h"

#include <RengaAPI/Materials.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/UserAttributeRegister.h>
#include <RengaAPI/UserAttributeDescription.h>
#include <RengaBase/AreaMeasure.h>
#include <RengaBase/LengthMeasure.h>
#include <RengaBase/VolumeMeasure.h>

ObjectPropertyViewBuilder::ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator)
  : m_pPropertyManagers(pPropertyManagers)
  , m_pTranslator(pTranslator)
{}

PropertyList ObjectPropertyViewBuilder::createUserAttributesProperties(rengaapi::ModelObject* pObject)
{
	PropertyList pResult;

  rengaapi::UserAttributeRegister userAttributeRegister = rengaapi::Project::userAttributeRegister();
  rengaapi::UserAttributeIdCollection attributeCollection = userAttributeRegister.attributes();

	// block signals before filling properties
	m_pPropertyManagers->m_pStringUserAttributeManager->blockSignals(true);
	m_pPropertyManagers->m_pDoubleUserAttributeManager->blockSignals(true);

	// check all attributes
  for (size_t i = 0; i < attributeCollection.size(); ++i)
  {
    const rengaapi::UserAttributeId& attributeId = attributeCollection.get(i);

		// check if objectType has attribute
    if (userAttributeRegister.typeHasAttribute(pObject->type(), attributeId))
    {
      rengaapi::UserAttributeDescription userAttributeDescription;

			// get attribute description
      if (userAttributeRegister.attributeDescription(attributeId, userAttributeDescription).code() == rengaapi::Status::Success)
      {
        rengaapi::UserAttributeValue userAttributeValue;

				// check if object has value for current attribute
        rengaapi::Status status = pObject->getUserAttributeValue(attributeId, userAttributeValue);
				QtProperty* pUserAttributeProperty = nullptr;
        QString attributeName = rengaStringToQString(userAttributeDescription.name());
				
				switch (userAttributeDescription.type())
				{
					case rengaapi::UserAttributeType::Double:
					{
						pUserAttributeProperty = m_pPropertyManagers->m_pDoubleUserAttributeManager->addProperty(attributeName);
						if (status.code() == rengaapi::Status::Success)
						{
							m_pPropertyManagers->m_pDoubleUserAttributeManager->setValue(pUserAttributeProperty, userAttributeValue.asDouble());
						}
						pUserAttributeProperty->setModified(true);
            pUserAttributeProperty->setData(rengaStringToQString(attributeId.id().toString()));
					}
					break;
					case rengaapi::UserAttributeType::String:
					{
						pUserAttributeProperty = m_pPropertyManagers->m_pStringUserAttributeManager->addProperty(attributeName);
						if (status.code() == rengaapi::Status::Success)
						{
							m_pPropertyManagers->m_pStringUserAttributeManager->setValue(pUserAttributeProperty, rengaStringToQString(userAttributeValue.asString()));
						}
						pUserAttributeProperty->setModified(true);
            pUserAttributeProperty->setData(rengaStringToQString(attributeId.id().toString()));
					}
					break;
					default:
						assert(false);
				}
				if (pUserAttributeProperty != nullptr)
					pResult.push_back(pUserAttributeProperty);
      }
    }
  }

	// unblock signals
	m_pPropertyManagers->m_pDoubleUserAttributeManager->blockSignals(false);
	m_pPropertyManagers->m_pStringUserAttributeManager->blockSignals(false);

	return pResult;
}

QString ObjectPropertyViewBuilder::getMaterialName(const rengaapi::MaterialId& materialId)
{
	rengaapi::Materials::MaterialType materialType= rengaapi::Materials::materialType(materialId);
	QString resultString = QString();
  switch(materialType)
  {
		case rengaapi::Materials::MaterialType::Grid:
		{
			rengaapi::GridMaterial gridMaterial;
			rengaapi::Materials::gridMaterial(materialId, gridMaterial);
			// grid materials are nameless, return collection name
			resultString = rengaStringToQString(materialId.collectionName());
		}
		break;
		case rengaapi::Materials::MaterialType::OneLayered:
		{
			rengaapi::Material material;
			rengaapi::Materials::material(materialId, material);
			resultString = rengaStringToQString(material.name_());
		}
		break;
		case rengaapi::Materials::MaterialType::MultiLayered:
		{
			rengaapi::LayeredMaterial layeredMaterial;
			rengaapi::Materials::layeredMaterial(materialId, layeredMaterial);
			resultString = rengaStringToQString(layeredMaterial.name_());
		}
		break;
	}
	return resultString;
}

void ObjectPropertyViewBuilder::setLengthMeasureOptional(PropertyList& insertPlace, const rengabase::LengthMeasureOptional& measure, QtProperty* pProperty)
{
	if (measure.hasValue())
	{
		m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, measure.getValue()->inMillimeters());
		insertPlace.push_back(pProperty);
	}
}

void ObjectPropertyViewBuilder::setAreaMeasureOptional(PropertyList& insertPlace, const rengabase::AreaMeasureOptional& measure, QtProperty* pProperty)
{
	if (measure.hasValue())
	{
		m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, measure.getValue()->inMeters2());
		insertPlace.push_back(pProperty);
	}
}

void ObjectPropertyViewBuilder::setVolumeMeasureOptional(PropertyList& insertPlace, const rengabase::VolumeMeasureOptional& measure, QtProperty* pProperty)
{
	if (measure.hasValue())
	{
		m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, measure.getValue()->inMeters3());
		insertPlace.push_back(pProperty);
	}
}

void ObjectPropertyViewBuilder::setOneLayeredMass(PropertyList& insertPlace, const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure, QtProperty* pProperty)
{
	assert(rengaapi::Materials::materialType(materialId) == rengaapi::Materials::MaterialType::OneLayered);

	if (!volumeMeasure.hasValue())
		return;

	rengaapi::Material material;
  rengaapi::Materials::material(materialId, material);
	if (material.isNull())
		return;

	double mass = material.density() * volumeMeasure.getValue()->inMeters3();
	m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, mass);

	insertPlace.push_back(pProperty);
}

void ObjectPropertyViewBuilder::setMultiLayeredMass(PropertyList& insertPlace, const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional> volumeMeasureCollection, QtProperty* pProperty)
{
	assert (rengaapi::Materials::materialType(materialId) == rengaapi::Materials::MaterialType::MultiLayered);

	rengaapi::LayeredMaterial layeredMaterial;
  rengaapi::Materials::layeredMaterial(materialId, layeredMaterial);
	if (layeredMaterial.isNull())
		return;

  rengaapi::MaterialLayersCollection materialLayers = layeredMaterial.layers();
	assert(volumeMeasureCollection.size() == materialLayers.size());

	double mass = 0.0;
	for(size_t i = 0; i < materialLayers.size(); ++i)
  {
		if (volumeMeasureCollection[i].hasValue())
		{
			rengaapi::Material materialLayer = materialLayers.get(i).material();
			if (!materialLayer.isNull())
				mass += materialLayer.density() * volumeMeasureCollection[i].getValue()->inMeters3();
		}
  }
	m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, mass);

	insertPlace.push_back(pProperty);
}