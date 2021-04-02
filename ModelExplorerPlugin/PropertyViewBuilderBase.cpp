#include "stdafx.h"

#include "PropertyViewBuilderBase.h"
#include "PropertyManager.h"
#include "GuidUtils.h"

#include <Renga/QuantityIds.h>


namespace
{
  template <typename TFunc>
  class CActionGuard
  {
  public:
    CActionGuard(const TFunc& func) : m_func(func) {}
    ~CActionGuard() { m_func(); }

  private:
    TFunc m_func;
  };

  template <typename TFunc>
  CActionGuard<TFunc> makeGuard(const TFunc& func)
  {
    return CActionGuard<TFunc>(func);
  }
}


PropertyViewBuilderBase::PropertyViewBuilderBase(bool disableProperties) : m_disableProperties(disableProperties)
{
}

PropertyList PropertyViewBuilderBase::createParametersInternal(
    PropertyManager& mngr,
    Renga::IParameterContainer& container)
{
  PropertyList result;
  // block signals before filling properties
  mngr.blockSignals(true);

  auto unblockSignalsGuard = makeGuard([&mngr] {mngr.blockSignals(false); });

  auto pIds = container.GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    const auto id = pIds->Get(i);

    auto pParameter = container.Get(id);
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
      pQtProperty = mngr.addValue(result, name, static_cast<bool>(pParameter->GetBoolValue()));
      break;
    case Renga::ParameterValueType::ParameterValueType_Int:
      pQtProperty = mngr.addValue(result, name, pParameter->GetIntValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_Double:
      pQtProperty = mngr.addValue(result, name, pParameter->GetDoubleValue());
      break;
    case Renga::ParameterValueType::ParameterValueType_String:
      pQtProperty = mngr.addValue(result, name, QString::fromWCharArray(pParameter->GetStringValue()));
      break;
    }

    if (pQtProperty)
    {
      if (!pParameter->HasValue())
        pQtProperty->setEnabled(false);

      const auto parameterIdString = QString::fromStdString((GuidToString(id)));
      pQtProperty->setData(parameterIdString);
    }
  }

  return result;
}

PropertyList PropertyViewBuilderBase::createPropertiesInternal(
    PropertyManager& mngr,
    Renga::IPropertyContainer& container)
{
  PropertyList result;
  // block signals before filling properties
  mngr.blockSignals(true);

  auto ids = container.GetIds();

  for (int i = 0; i < ids->Count; ++i)
  {
    auto id = ids->Get(i);
    auto pProperty = container.Get(id);

    if (!pProperty)
      continue;

    const auto attributeName = QString::fromWCharArray(pProperty->Name);
    const auto propertyIdString = QString::fromStdString((GuidToString(id)));


    // TODO: add support for all property types
    QtProperty* pQtProperty(nullptr);
    switch (pProperty->Type)
    {
    case Renga::PropertyType::PropertyType_Double:
      pQtProperty = mngr.addValue(result, attributeName, pProperty->GetDoubleValue());
      break;
    case Renga::PropertyType::PropertyType_String:
      pQtProperty = mngr.addValue(result, attributeName, QString::fromWCharArray(pProperty->GetStringValue()));
      break;
    default:
      continue;
    }
    
    pQtProperty->setEnabled(!m_disableProperties);
    pQtProperty->setData(propertyIdString);
  }

  // unblock signals
  mngr.blockSignals(false);

  return result;
}

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
