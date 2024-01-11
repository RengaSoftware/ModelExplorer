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
  m_pEnumManager = new QtEnumPropertyManager(pParent);

  auto pIntPropertyFactory    = new QtSpinBoxFactory(pParent, readOnly);
  auto pDoublePropertyFactory = new QtLineEditFactory(pParent, readOnly);
  auto pStringPropertyFactory = new QtLineEditFactory(pParent, readOnly);
  auto pEnumPropertyFactory   = new QtEnumEditorFactory(pParent);
  auto pBoolPropertyFactory   = new QtCheckBoxFactory(pParent, readOnly);

  pParent->setFactoryForManager(m_pBoolManager, pBoolPropertyFactory);
  pParent->setFactoryForManager(m_pIntManager, pIntPropertyFactory);
  pParent->setFactoryForManager(m_pDoubleManager, pDoublePropertyFactory);
  pParent->setFactoryForManager(m_pStringManager, pStringPropertyFactory);
  pParent->setFactoryForManager(m_pEnumManager, pEnumPropertyFactory);
}

void PropertyManager::clear()
{
  m_pBoolManager->clear();
  m_pIntManager->clear();
  m_pDoubleManager->clear();
  m_pStringManager->clear();
  m_pEnumManager->clear();
}

void PropertyManager::blockSignals(bool b) const
{
  m_pBoolManager->blockSignals(b);
  m_pIntManager->blockSignals(b);
  m_pStringManager->blockSignals(b);
  m_pDoubleManager->blockSignals(b);
  m_pEnumManager->blockSignals(b);
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

QtProperty * PropertyManager::addValue(const QString& name, int valueIdx, const QStringList& values) const
{
  auto pManager = m_pEnumManager;
  auto pProperty = pManager->addProperty(name);
  pManager->setEnumNames(pProperty, values);
  pManager->setValue(pProperty, valueIdx);
  return pProperty;
}

PropertyList PropertyManager::properties() const
{
  auto propertySetToList = [](const QSet<QtProperty*>& set, PropertyList& list)
  {
    list.insert(list.end(), set.cbegin(), set.cend());
  };

  auto result = PropertyList();

  propertySetToList(m_pDoubleManager->properties(), result);
  propertySetToList(m_pStringManager->properties(), result);
  propertySetToList(m_pBoolManager->properties(), result);
  propertySetToList(m_pIntManager->properties(), result);
  propertySetToList(m_pEnumManager->properties(), result);
  return result;
}
