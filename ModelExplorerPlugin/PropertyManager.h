//
// Copyright �Renga Software� LLC, 2016. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "PropertyList.h"

#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>


class PropertyView;

// TODO: rename this, or devide - responsibility is to create raw Qt properties and hold them
class PropertyManager
{
public:
  PropertyManager();

  void init(PropertyView* pParent, bool readOnly);
  void clear();
  void blockSignals(bool b) const;

  QtProperty* addValue(const QString& name, const bool value) const;
  QtProperty* addValue(const QString& name, const int value) const;
  QtProperty* addValue(const QString& name, const double value) const;
  QtProperty* addValue(const QString& name, const QString& value) const;
  QtProperty* addValue(const QString& name, int valueIdx, const QStringList& values) const;
  PropertyList properties() const;

// TODO: make private
public:
  QtBoolPropertyManager* m_pBoolManager;
  QtIntPropertyManager* m_pIntManager;
  QtStringPropertyManager* m_pDoubleManager;
  QtStringPropertyManager* m_pStringManager;
  QtEnumPropertyManager* m_pEnumManager;
};