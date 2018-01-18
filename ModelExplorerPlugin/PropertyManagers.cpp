#include "stdafx.h"
#include "PropertyManagers.h"


PropertyManagers::PropertyManagers() :
  m_pIntManager(nullptr),
  m_pDoubleManager(nullptr),
  m_pStringManager(nullptr),
  m_pDoubleUserAttributeManager(nullptr),
  m_pStringUserAttributeManager(nullptr)
{
}

QtProperty* PropertyManagers::addValue(const QString& name, const double value) const
{
  auto& pManager = m_pDoubleManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}

QtProperty* PropertyManagers::addValue(const QString& name, const QString& value) const
{
  auto& pManager = m_pStringManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}


void PropertyManagers::addValue(PropertyList& propertyList, const QString& name, const int value) const
{
  auto& pManager = m_pIntManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  propertyList.push_back(pProperty);
}

void PropertyManagers::addValue(PropertyList& propertyList, const QString& name, const double value) const
{
  propertyList.push_back(addValue(name, value));
}

void PropertyManagers::addValue(PropertyList& propertyList, const QString& name, const QString& value) const
{
  propertyList.push_back(addValue(name, value));
}

void PropertyManagers::addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityContainerPtr pQuantityContainer, const GUID quantityId) const {
  addValue(propertyList, name, pQuantityContainer->GetQuantity(quantityId));
}

void PropertyManagers::addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityPtr pQuantity) const
{
  if (!pQuantity)
    return;

  const auto quantityType = pQuantity->GetType();
  if (quantityType == Renga::QuantityType::QuantityType_Unknown)
    return;

  if (quantityType == Renga::QuantityType::QuantityType_Count) {
    addValue(propertyList, name, pQuantity->AsCount());
    return;
  }

  double value;
  switch (quantityType)
  {
  case Renga::QuantityType::QuantityType_Length:
    value = pQuantity->AsLength(Renga::LengthUnit::LengthUnit_Millimeters);
    break;
  case Renga::QuantityType::QuantityType_Area:
    value = pQuantity->AsArea(Renga::AreaUnit::AreaUnit_Meters2);
    break;
  case Renga::QuantityType::QuantityType_Volume:
    value = pQuantity->AsVolume(Renga::VolumeUnit::VolumeUnit_Meters3);
    break;
  case Renga::QuantityType::QuantityType_Mass:
    value = pQuantity->AsMass(Renga::MassUnit::MassUnit_Kilograms);
    break;
  }
  addValue(propertyList, name, value);
}
