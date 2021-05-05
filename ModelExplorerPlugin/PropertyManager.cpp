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
