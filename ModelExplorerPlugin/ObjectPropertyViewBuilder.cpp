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

ObjectPropertyViewBuilder::ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers)
  : m_pPropertyManagers(pPropertyManagers)
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

void ObjectPropertyViewBuilder::setLengthMeasureOptional(PropertyList& insertPlace, const rengabase::LengthMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit)
{
  if (measure.hasValue())
  {
    const rengabase::LengthMeasure* lengthMeasure = measure.getValue();
    double propertyValue;
    switch (unit)
    {
    case Meter:
      propertyValue = lengthMeasure->inMeters();
      break;
    case Centimeter:
      propertyValue = lengthMeasure->inCentimeters();
      break;
    case Inch:
      propertyValue = lengthMeasure->inInches();
      break;
    case Millimeter:
    default:
      propertyValue = lengthMeasure->inMillimeters();
      break;
    }
    m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, propertyValue);
    insertPlace.push_back(pProperty);
  }
}

void ObjectPropertyViewBuilder::setAreaMeasureOptional(PropertyList& insertPlace, const rengabase::AreaMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit)
{
  if (measure.hasValue())
  {
    const rengabase::AreaMeasure* areaMeasure = measure.getValue();
    double propertyValue;
    switch (unit)
    {
    case Centimeter:
      propertyValue = areaMeasure->inCentimeters2();
      break;
    case Millimeter:
      propertyValue = areaMeasure->inMillimeters2();
      break;
    case Meter:
    default:
      propertyValue = areaMeasure->inMeters2();
      break;
    }
    m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, propertyValue);
    insertPlace.push_back(pProperty);
  }
}

void ObjectPropertyViewBuilder::setVolumeMeasureOptional(PropertyList& insertPlace, const rengabase::VolumeMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit)
{
  if (measure.hasValue())
  {
    const rengabase::VolumeMeasure* volumeMeasure = measure.getValue();
    double propertyValue;
    switch (unit)
    {
    case Centimeter:
      propertyValue = volumeMeasure->inCentimeters3();
      break;
    case Millimeter:
      propertyValue = volumeMeasure->inMillimeters3();
      break;
    case Meter:
    default:
      propertyValue = volumeMeasure->inMeters3();
      break;
    }
    m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, propertyValue);
    insertPlace.push_back(pProperty);
  }
}

void ObjectPropertyViewBuilder::setMassMeasureOptional(PropertyList & insertPlace, const rengabase::MassMeasureOptional & measure, QtProperty * pProperty, MassMeasureUnit unit)
{
  if (measure.hasValue())
  {
    const rengabase::MassMeasure* massMeasure = measure.getValue();
    double propertyValue;

    switch (unit)
    {
    case ObjectPropertyViewBuilder::kg:
      propertyValue = massMeasure->inKilograms();
      break;
    case ObjectPropertyViewBuilder::ton:
      propertyValue = massMeasure->inTons();
      break;
    default:
      propertyValue = massMeasure->inKilograms();
      break;
    }
    m_pPropertyManagers->m_pDoubleManager->setValue(pProperty, propertyValue);
    insertPlace.push_back(pProperty);
  }
}