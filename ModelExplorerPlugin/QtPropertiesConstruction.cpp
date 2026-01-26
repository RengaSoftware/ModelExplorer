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
#include "ScopeGuard.h"

#include <Renga/Quantities.h>


void qtPropertiesFromRengaParameters(
    PropertyManager& mngr,
    Renga::IParameterContainer& container,
    EntityByParameterIdNameGetter nameGetter)
{
  // block signals before filling properties
  mngr.blockSignals(true);

  auto unblockSignalsGuard = makeGuard([&mngr] {mngr.blockSignals(false); });

  auto pIds = container.GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    const auto id = pIds->Get(i);
    auto pParameter = container.Get(id);

    if (!pParameter->HasValue)
      continue;

    auto pDefinition = pParameter->Definition;

    auto name = QString::fromStdWString(pDefinition->Text.operator wchar_t *());

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
    {
      // Try to show related entity name instead of id
      // If the name is empty - show id
      // TODO: show both id and name, refactoring needed
      if (pDefinition->GetParameterType() == Renga::ParameterType_IntID)
      {
        auto entityName = nameGetter(id, pParameter->GetIntValue());
        if (!entityName.isEmpty())
        {
          pQtProperty = mngr.addValue(name, entityName);
          break;
        }
      }
      
      pQtProperty = mngr.addValue(name, pParameter->GetIntValue());
      break;
    }
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
  
  addValue(QApplication::translate("me_mo", "overallWidth"), Quantities::OverallWidth);
  addValue(QApplication::translate("me_mo", "overallHeight"), Quantities::OverallHeight);
  addValue(QApplication::translate("me_mo", "overallLength"), Quantities::OverallLength);
  addValue(QApplication::translate("me_mo", "thickness"), Quantities::NominalThickness);
  addValue(QApplication::translate("me_mo", "length"), Quantities::NominalLength);
  addValue(QApplication::translate("me_mo", "width"), Quantities::NominalWidth);
  addValue(QApplication::translate("me_mo", "height"), Quantities::NominalHeight);
  addValue(QApplication::translate("me_mo", "perimeter"), Quantities::Perimeter);
  addValue(QApplication::translate("me_mo", "volume"), Quantities::Volume);
  addValue(QApplication::translate("me_mo", "netVolume"), Quantities::NetVolume);
  addValue(QApplication::translate("me_mo", "mass"), Quantities::NetMass);
  addValue(QApplication::translate("me_mo", "crossSectionOverallWidth"), Quantities::CrossSectionOverallWidth);
  addValue(QApplication::translate("me_mo", "crossSectionOverallHeight"), Quantities::CrossSectionOverallHeight);
  addValue(QApplication::translate("me_mo", "netCrossSectionArea"), Quantities::NetCrossSectionArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceArea"), Quantities::InnerSurfaceArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceInternalArea"), Quantities::InnerSurfaceInternalArea);
  addValue(QApplication::translate("me_mo", "innerSurfaceExternalArea"), Quantities::InnerSurfaceExternalArea);
  addValue(QApplication::translate("me_mo", "outerSurfaceArea"), Quantities::OuterSurfaceArea);
  addValue(QApplication::translate("me_mo", "area"), Quantities::Area);
  addValue(QApplication::translate("me_mo", "nominalArea"), Quantities::NominalArea);
  addValue(QApplication::translate("me_mo", "netArea"), Quantities::NetArea);
  addValue(QApplication::translate("me_mo", "netFloorArea"), Quantities::NetFloorArea);
  addValue(QApplication::translate("me_mo", "netFootprintArea"), Quantities::NetFootprintArea);
  addValue(QApplication::translate("me_mo", "netSideArea"), Quantities::NetSideArea);
  addValue(QApplication::translate("me_mo", "totalSurfaceArea"), Quantities::TotalSurfaceArea);
  addValue(QApplication::translate("me_mo", "grossVolume"), Quantities::GrossVolume);
  addValue(QApplication::translate("me_mo", "grossFloorArea"), Quantities::GrossFloorArea);
  addValue(QApplication::translate("me_mo", "grossPerimeter"), Quantities::GrossPerimeter);
  addValue(QApplication::translate("me_mo", "numberOfRiser"), Quantities::NumberOfRiser);
  addValue(QApplication::translate("me_mo", "numberOfTreads"), Quantities::NumberOfTreads);
  addValue(QApplication::translate("me_mo", "riserHeight"), Quantities::RiserHeight);
  addValue(QApplication::translate("me_mo", "treadLength"), Quantities::TreadLength);
  addValue(QApplication::translate("me_reinforcement", "totalRebarLength"), Quantities::TotalRebarLength);
  addValue(QApplication::translate("me_reinforcement", "totalRebarMass"), Quantities::TotalRebarMass);
}
