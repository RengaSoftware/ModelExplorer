#include "stdafx.h"

#include "PropertyViewModelObjectSource.h"

#include <Renga/ObjectTypes.h>

#include "OpeningPropertyViewBuilder.h"
#include "RoofPropertyViewBuilder.h"
#include "LevelPropertyViewBuilder.h"
#include "WallPropertyViewBuilder.h"
#include "ColumnPropertyViewBuilder.h"
#include "FloorPropertyViewBuilder.h"
#include "BeamPropertyViewBuilder.h"
#include "RampPropertyViewBuilder.h"
#include "WindowPropertyViewBuilder.h"
#include "DoorPropertyViewBuilder.h"
#include "RailingPropertyViewBuilder.h"
#include "RoomPropertyViewBuilder.h"
#include "StairPropertyViewBuilder.h"
#include "IsolatedFoundationPropertyViewBuilder.h"
#include "WallFoundationPropertyViewBuilder.h"
#include "AssemblyInstancePropertyViewBuilder.h"
#include "ElementPropertyViewBuilder.h"
#include "EquipmentPropertyViewBuilder.h"
#include "PlumbingFixturePropertyViewBuilder.h"


PropertyViewModelObjectSource::PropertyViewModelObjectSource(
  Renga::IApplicationPtr pApplication,
  Renga::IModelObjectPtr pModelObject) :
  m_pApplication(pApplication),
  m_pModelObject(pModelObject)
{
}

IPropertyViewBuilder* PropertyViewModelObjectSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  if (m_pModelObject == nullptr)
    return nullptr;

  const auto objectType = m_pModelObject->GetObjectType();

  IPropertyViewBuilder* pPropertyViewBuilder = nullptr;
  
  if (objectType == Renga::ObjectTypes::Level)
    pPropertyViewBuilder = new LevelPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Wall)
    pPropertyViewBuilder = new WallPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Column)
    pPropertyViewBuilder = new ColumnPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Floor)
    pPropertyViewBuilder = new FloorPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Opening)
    pPropertyViewBuilder = new OpeningPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Roof)
    pPropertyViewBuilder = new RoofPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Beam)
    pPropertyViewBuilder = new BeamPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Stair)
    pPropertyViewBuilder = new StairPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Ramp)
    pPropertyViewBuilder = new RampPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Window)
    pPropertyViewBuilder = new WindowPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Door)
    pPropertyViewBuilder = new DoorPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Railing)
    pPropertyViewBuilder = new RailingPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Room)
    pPropertyViewBuilder = new RoomPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::IsolatedFoundation)
    pPropertyViewBuilder = new IsolatedFoundationPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::WallFoundation)
    pPropertyViewBuilder = new WallFoundationPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::AssemblyInstance)
    pPropertyViewBuilder = new AssemblyInstancePropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Element)
    pPropertyViewBuilder = new ElementPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Equipment)
    pPropertyViewBuilder = new EquipmentPropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::PlumbingFixture)
    pPropertyViewBuilder = new PlumbingFixturePropertyViewBuilder(pPropertyManagers, m_pApplication, m_pModelObject);

  return pPropertyViewBuilder;
}

Renga::IPropertyPtr PropertyViewModelObjectSource::getUserDefinedProperty(GUID propertyId)
{
  if (m_pModelObject == nullptr)
    return nullptr;

  auto propertyContainer = m_pModelObject->GetProperties();

  return propertyContainer->Get(propertyId);
}
