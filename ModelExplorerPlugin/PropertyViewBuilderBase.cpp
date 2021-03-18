#include "stdafx.h"

#include "PropertyViewBuilderBase.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


PropertyList PropertyViewBuilderBase::createQuantitiesInternal(
    PropertyManager& mngr,
    Renga::IQuantityContainer& container)
{
  using namespace Renga;
  PropertyList result;
  auto addValue = [&result, &mngr, &container](QString name, GUID id)
  {
    mngr.addValue(result, name, container.Get(id));
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
  addValue(QApplication::translate("me_mo", "crossSectionArea"), QuantityIds::CrossSectionArea);
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
  return result;
}
