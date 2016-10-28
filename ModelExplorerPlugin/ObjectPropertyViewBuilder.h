//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "IObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

#include <qteditorfactory.h>

#include <RengaAPI/MaterialId.h>
#include <RengaBase/LengthMeasureOptional.h>
#include <RengaBase/AreaMeasureOptional.h>
#include <RengaBase/VolumeMeasureOptional.h>

class ObjectPropertyViewBuilder : public IObjectPropertyViewBuilder
{
public:
	ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator);

	virtual PropertyMap createParametersProperties(rengaapi::ModelObject* pObject) = 0;
	virtual PropertyMap createQuantitiesProperties(rengaapi::ModelObject* pObject) = 0;
	PropertyMap createUserAttributesProperties(rengaapi::ModelObject* pObject);

	static QString getMaterialName(const rengaapi::MaterialId& materialId);

	void setLengthMeasureOptional(PropertyMap& insertPlace, const rengabase::LengthMeasureOptional& measure, QtProperty* pProperty);
	void setAreaMeasureOptional(PropertyMap& insertPlace, const rengabase::AreaMeasureOptional& measure, QtProperty* pProperty);
	void setVolumeMeasureOptional(PropertyMap& insertPlace, const rengabase::VolumeMeasureOptional& measure, QtProperty* pProperty);

	void setOneLayeredMass(PropertyMap& insertPlace, const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure, QtProperty* pProperty);
	void setMultiLayeredMass(PropertyMap& insertPlace, const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional> volumeMeasureCollection, QtProperty* pProperty);

protected:
  const PropertyManagers* m_pPropertyManagers;
  const QTranslator* m_pTranslator;
};