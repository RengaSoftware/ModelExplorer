#include "stdafx.h"
#include "PropertyManager.h"
#include "PropertyView.h"

#include <qteditorfactory.h>


const uint c_defaultPrecision = 2;
const uint c_userAttrPrecision = 15;

PropertyManager::PropertyManager() :
  m_pBoolManager(nullptr),
  m_pIntManager(nullptr),
  m_pDoubleManager(nullptr),
  m_pStringManager(nullptr)
{
}

void PropertyManager::init(PropertyView* pParent, bool readOnly)
{
  m_pBoolManager = new QtBoolPropertyManager(pParent);
  m_pIntManager = new QtIntPropertyManager(pParent);
  if (readOnly)
    m_pDoubleManager = new QtStringPropertyManager(pParent, QtStringPropertyManager::valueTupe::doubleType, c_defaultPrecision, false);
  else
    m_pDoubleManager = new QtStringPropertyManager(pParent, QtStringPropertyManager::valueTupe::doubleType, c_userAttrPrecision, true);

  m_pStringManager = new QtStringPropertyManager(pParent);

  auto pIntPropertyFactory = new QtSpinBoxFactory(pParent, readOnly);
  auto pDoublePropertyFactory = new QtLineEditFactory(pParent, readOnly);
  auto pStringPropertyFactory = new QtLineEditFactory(pParent, readOnly);

  pParent->setFactoryForManager(m_pBoolManager, new QtCheckBoxFactory(pParent));
  pParent->setFactoryForManager(m_pIntManager, pIntPropertyFactory);
  pParent->setFactoryForManager(m_pDoubleManager, pDoublePropertyFactory);
  pParent->setFactoryForManager(m_pStringManager, pStringPropertyFactory);
}

void PropertyManager::clear()
{
  m_pBoolManager->clear();
  m_pIntManager->clear();
  m_pDoubleManager->clear();
  m_pStringManager->clear();
}

void PropertyManager::blockSignals(bool b) const
{
  m_pBoolManager->blockSignals(b);
  m_pIntManager->blockSignals(b);
  m_pStringManager->blockSignals(b);
  m_pDoubleManager->blockSignals(b);
}

QtProperty* PropertyManager::addValue(const QString& name, const bool value) const
{
  auto pManager = m_pBoolManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}

QtProperty* PropertyManager::addValue(const QString& name, const int value) const
{
  auto pManager = m_pIntManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}

QtProperty* PropertyManager::addValue(const QString& name, const double value) const
{
  auto pManager = m_pDoubleManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}

QtProperty* PropertyManager::addValue(const QString& name, const QString& value) const
{
  auto pManager = m_pStringManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setValue(pProperty, value);
  return pProperty;
}

QtProperty* PropertyManager::addValue(PropertyList& propertyList, const QString& name, const bool value) const
{
  auto pProperty = addValue(name, value);
  propertyList.push_back(pProperty);
  return pProperty;
}

QtProperty* PropertyManager::addValue(PropertyList& propertyList, const QString& name, const int value) const
{
  auto pProperty = addValue(name, value);
  propertyList.push_back(pProperty);
  return pProperty;
}

QtProperty* PropertyManager::addValue(PropertyList& propertyList, const QString& name, const double value) const
{
  auto pProperty = addValue(name, value);
  propertyList.push_back(pProperty);
  return pProperty;
}

QtProperty* PropertyManager::addValue(PropertyList& propertyList, const QString& name, const QString& value) const
{
  auto pProperty = addValue(name, value);
  propertyList.push_back(pProperty);
  return pProperty;
}

QtProperty* PropertyManager::addValue(PropertyList& propertyList, const QString& name, Renga::IQuantityPtr pQuantity) const
{
  if (!pQuantity)
    return nullptr;

  const auto quantityType = pQuantity->GetType();
  if (quantityType == Renga::QuantityType::QuantityType_Unknown)
    return nullptr;

  if (quantityType == Renga::QuantityType::QuantityType_Count) {
    addValue(propertyList, name, pQuantity->AsCount());
    return nullptr;
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
  return addValue(propertyList, name, value);
}
