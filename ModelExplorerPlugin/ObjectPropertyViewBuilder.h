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
#include <RengaBase/MassMeasureOptional.h>

class ObjectPropertyViewBuilder : public IObjectPropertyViewBuilder
{
public:
	ObjectPropertyViewBuilder(const PropertyManagers* pPropertyManagers);

	virtual PropertyList createParametersProperties(rengaapi::ModelObject* pObject) = 0;
	virtual PropertyList createQuantitiesProperties(rengaapi::ModelObject* pObject) = 0;
	PropertyList createUserAttributesProperties(rengaapi::ModelObject* pObject);

	static QString getMaterialName(const rengaapi::MaterialId& materialId);

  enum MeasureUnit
  {
    Meter = 0,
    Centimeter = 1,
    Millimeter = 2,
    Inch = 3
  };
  void setLengthMeasureOptional(PropertyList& insertPlace, const rengabase::LengthMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit = MeasureUnit::Millimeter);
  void setAreaMeasureOptional(PropertyList& insertPlace, const rengabase::AreaMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit = MeasureUnit::Meter);
  void setVolumeMeasureOptional(PropertyList& insertPlace, const rengabase::VolumeMeasureOptional& measure, QtProperty* pProperty, MeasureUnit unit = MeasureUnit::Meter);

  enum MassMeasureUnit
  {
    kg,
    ton
  };
  void setMassMeasureOptional(PropertyList& insertPlace, const rengabase::MassMeasureOptional& measure, QtProperty* pProperty, MassMeasureUnit unit = MassMeasureUnit::kg);

protected:
  const PropertyManagers* m_pPropertyManagers;
};