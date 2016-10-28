//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>

class PropertyManagers
{
public:
  PropertyManagers()
    : m_pIntManager(nullptr)
    , m_pDoubleManager(nullptr)
    , m_pStringManager(nullptr)
    , m_pDoubleUserAttributeManager(nullptr)
    , m_pStringUserAttributeManager(nullptr)
  {}

  QtIntPropertyManager* m_pIntManager;
  QtStringPropertyManager* m_pDoubleManager;
  QtStringPropertyManager* m_pStringManager;
  QtStringPropertyManager* m_pDoubleUserAttributeManager;
  QtStringPropertyManager* m_pStringUserAttributeManager;
};