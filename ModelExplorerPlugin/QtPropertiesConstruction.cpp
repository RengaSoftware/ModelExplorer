//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include "COMUtils.h"
#include "PropertyManager.h"
#include "QtPropertiesConstruction.h"
#include "RengaModelUtils.h"
#include "ScopeGuard.h"

#include <Renga/QuantityIds.h>


void qtPropertiesFromRengaParameters(
    PropertyManager& mngr,
    Renga::IParameterContainer& container)
{
  // block signals before filling properties
  mngr.blockSignals(true);

  auto unblockSignalsGuard = makeGuard([&mngr] {mngr.blockSignals(false); });

  auto pIds = container.GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    const auto id = pIds->Get(i);
    auto pParameter = container.Get(id);

    if (!pParameter->HasValue())
      continue;

    auto pDefinition = pParameter->Definition;

    QString name = QString::fromStdWString(pDefinition->Name.operator wchar_t *());

    switch (pDefinition->GetParameterType())
    {
    case Renga::ParameterType::ParameterType_Angle:
      name += ", " + QApplication::translate("me_mo", "angle_dimension");
      break;
    case Renga::ParameterType::ParameterType_Length:
      name += ", " + QApplication::translate("me_mo", "length_dimension");
      break;
    }

    QtProperty* pQtProperty(nullptr);
    switch (pParameter->GetValueType())
    {
    case Renga::ParameterValueType::ParameterValueType_Bool:
      pQtProperty = mngr.addValue(name, static_cast<bool>(pParameter->GetBoolValue()));
      break;
    case Renga::ParameterValueType::ParameterValueType_Int:
      pQtProperty = mngr.addValue(name, pParameter->GetIntValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_Double:
      pQtProperty = mngr.addValue(name, pParameter->GetDoubleValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_String:
      pQtProperty = mngr.addValue(name, QString::fromWCharArray(pParameter->GetStringValue()));
      break;
    }

    if (pQtProperty)
    {
      const auto parameterIdString = QString::fromStdString((GuidToString(id)));
      pQtProperty->setData(parameterIdString);
    }
  }
}

void qtPropertiesFromRengaProperties(
    PropertyManager& mngr,
    Renga::IPropertyManager& rengaPropertyManager,
    Renga::IPropertyContainer& container)
{
  // block signals before filling properties
  mngr.blockSignals(true);

  auto ids = container.GetIds();
  for (int i = 0; i < ids->Count; ++i)
  {
    auto id = ids->Get(i);
    auto pProperty = container.Get(id);

    if (!pProperty)
      continue;

    auto name = QString::fromWCharArray(pProperty->Name);
    const auto propertyIdString = QString::fromStdString((GuidToString(id)));


    // TODO: add support for all property types
    QtProperty* pQtProperty(nullptr);
    switch (pProperty->Type)
    {
    case Renga::PropertyType::PropertyType_Angle:
      name += ", " + QApplication::translate("me_mo", "angle_dimension");
      pQtProperty = mngr.addValue(name, pProperty->GetAngleValue(Renga::AngleUnit::AngleUnit_Degrees));
      break;
    case Renga::PropertyType::PropertyType_Length:
      name += ", " + QApplication::translate("me_mo", "length_dimension");
      pQtProperty = mngr.addValue(name, pProperty->GetLengthValue(Renga::LengthUnit::LengthUnit_Millimeters));
      break;
    case Renga::PropertyType::PropertyType_Area:
      name += ", " + QApplication::translate("me_mo", "m2");
      pQtProperty = mngr.addValue(name, pProperty->GetAreaValue(Renga::AreaUnit::AreaUnit_Meters2));
      break;
    case Renga::PropertyType::PropertyType_Volume:
      name += ", " + QApplication::translate("me_mo", "m3");
      pQtProperty = mngr.addValue(name, pProperty->GetVolumeValue(Renga::VolumeUnit::VolumeUnit_Meters3));
      break;
    case Renga::PropertyType::PropertyType_Mass:
      name += ", " + QApplication::translate("me_mo", "kg");
      pQtProperty = mngr.addValue(name, pProperty->GetMassValue(Renga::MassUnit::MassUnit_Kilograms));
      break;
    case Renga::PropertyType::PropertyType_Double:
      pQtProperty = mngr.addValue(name, pProperty->GetDoubleValue());
      break;
    case Renga::PropertyType::PropertyType_String:
      pQtProperty = mngr.addValue(name, QString::fromWCharArray(pProperty->GetStringValue()));
      break;
    case Renga::PropertyType::PropertyType_Boolean:
      pQtProperty = mngr.addValue(name, (bool)pProperty->GetBooleanValue());
      break;
    case Renga::PropertyType::PropertyType_Integer:
      pQtProperty = mngr.addValue(name, pProperty->GetIntegerValue());
      break;
    case Renga::PropertyType::PropertyType_Enumeration:
    {
      auto pDescription = rengaPropertyManager.GetPropertyDescription2(pProperty->Id);
      auto values = safeArrayToQStringList(pDescription->GetEnumerationItems());
      auto valueIndex = values.indexOf(QString::fromWCharArray(pProperty->GetEnumerationValue()), 0);
      pQtProperty = mngr.addValue(name, valueIndex, values);
    }
      break;
    case Renga::PropertyType::PropertyType_Logical:
    {
      auto values = getLogicalValueStringList();
      auto valueIndex = getLogicalValueIndex(pProperty->GetLogicalValue());
      pQtProperty = mngr.addValue(name, valueIndex, values);
    }
    break;
    default:
      continue;
    }
    
    // TODO: text color should depend on control enable state, for now we use "modified" flag to ungrey text in edit box
    pQtProperty->setModified(true);
    pQtProperty->setData(propertyIdString);
  }

  // unblock signals
  mngr.blockSignals(false);
}

void qtPropertiesFromRengaQuantities(PropertyManager& mngr, Renga::IQuantityContainer& container)
{
  using namespace Renga;

  auto addValue = [&mngr, &container](QString name, GUID id)
  {
    auto pQuantity = container.Get(id);

    if (!pQuantity)
      return;

    switch (pQuantity->GetType())
    {
    case Renga::QuantityType::QuantityType_Length:
      mngr.addValue(name, pQuantity->AsLength(Renga::LengthUnit::LengthUnit_Millimeters));
      break;
    case Renga::QuantityType::QuantityType_Area:
      mngr.addValue(name, pQuantity->AsArea(Renga::AreaUnit::AreaUnit_Meters2));
      break;
    case Renga::QuantityType::QuantityType_Volume:
      mngr.addValue(name, pQuantity->AsVolume(Renga::VolumeUnit::VolumeUnit_Meters3));
      break;
    case Renga::QuantityType::QuantityType_Mass:
      mngr.addValue(name, pQuantity->AsMass(Renga::MassUnit::MassUnit_Kilograms));
      break;
    case Renga::QuantityType::QuantityType_Count:
      mngr.addValue(name, pQuantity->AsCount());
      break;
    }
  };
  
  addValue(QApplication::translate("me_mo", "overallWidth"), QuantityIds::OverallWidth);
  addValue(QApplication::translate("me_mo", "overallHeight"), QuantityIds::OverallHeight);
  addValue(QApplication::translate("me_mo", "overallLength"), QuantityIds::OverallLength);
  addValue(QApplication::translate("me_mo", "thickness"), QuantityIds::NominalThickness);
  addValue(QApplication::translate("me_mo", "length"), QuantityIds::NominalLength);
  addValue(QApplication::translate("me_mo", "width"), QuantityIds::NominalWidth);
  addValue(QApplication::translate("me_mo", "height"), QuantityIds::NominalHeight);
  addValue(QApplication::translate("me_mo", "perimeter"), QuantityIds::Perimeter);
  addValue(QApplication::translate("me_mo", "volume"), QuantityIds::Volume);
  addValue(QApplication::translate("me_mo", "netVolume"), QuantityIds::NetVolume);
  addValue(QApplication::translate("me_mo", "mass"), QuantityIds::NetMass);
  addValue(QApplication::translate("me_mo", "crossSectionOverallWidth"), QuantityIds::CrossSectionOverallWidth);
  addValue(QApplication::translate("me_mo", "crossSectionOverallHeight"), QuantityIds::CrossSectionOverallHeight);
  addValue(QApplication::translate("me_mo", "netCrossSectionArea"), QuantityIds::NetCrossSectionArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceArea"), QuantityIds::InnerSurfaceArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceInternalArea"), QuantityIds::InnerSurfaceInternalArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceExternalArea"), QuantityIds::InnerSurfaceExternalArea);
  addValue(QApplication::translate("me_mo", "outerSurfaceArea"), QuantityIds::OuterSurfaceArea);
  addValue(QApplication::translate("me_mo", "area"), QuantityIds::Area);
  addValue(QApplication::translate("me_mo", "nominalArea"), QuantityIds::NominalArea);
  addValue(QApplication::translate("me_mo", "netArea"), QuantityIds::NetArea);
  addValue(QApplication::translate("me_mo", "netFloorArea"), QuantityIds::NetFloorArea);
  addValue(QApplication::translate("me_mo", "netFootprintArea"), QuantityIds::NetFootprintArea);
  addValue(QApplication::translate("me_mo", "netSideArea"), QuantityIds::NetSideArea);
  addValue(QApplication::translate("me_mo", "totalSurfaceArea"), QuantityIds::TotalSurfaceArea);
  addValue(QApplication::translate("me_mo", "grossVolume"), QuantityIds::GrossVolume);
  addValue(QApplication::translate("me_mo", "grossFloorArea"), QuantityIds::GrossFloorArea);
  addValue(QApplication::translate("me_mo", "grossPerimeter"), QuantityIds::GrossPerimeter);
  addValue(QApplication::translate("me_mo", "numberOfRiser"), QuantityIds::NumberOfRiser);
  addValue(QApplication::translate("me_mo", "numberOfTreads"), QuantityIds::NumberOfTreads);
  addValue(QApplication::translate("me_mo", "riserHeight"), QuantityIds::RiserHeight);
  addValue(QApplication::translate("me_mo", "treadLength"), QuantityIds::TreadLength);
  addValue(QApplication::translate("me_reinforcement", "totalRebarLength"), QuantityIds::TotalRebarLength);
  addValue(QApplication::translate("me_reinforcement", "totalRebarMass"), QuantityIds::TotalRebarMass);
}
