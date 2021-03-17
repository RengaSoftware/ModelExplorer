#include "stdafx.h"

#include "PropertyViewModelObjectSource.h"

#include <Renga/ObjectTypes.h>

#include "LevelPropertyViewBuilder.h"
#include "RoomPropertyViewBuilder.h"


PropertyViewModelObjectSource::PropertyViewModelObjectSource(
  Renga::IApplicationPtr pApplication,
  Renga::IModelObjectPtr pModelObject) :
  m_pApplication(pApplication),
  m_pModelObject(pModelObject)
{
}

std::unique_ptr<IPropertyViewBuilder> PropertyViewModelObjectSource::createPropertyViewBuilder(PropertyManagers* pPropertyManagers)
{
  if (m_pModelObject == nullptr)
    return nullptr;

  const auto objectType = m_pModelObject->GetObjectType();

  if (objectType == Renga::ObjectTypes::Undefined)
    return nullptr;

  if (objectType == Renga::ObjectTypes::Level)
    return std::make_unique<LevelPropertyViewBuilder>(pPropertyManagers, m_pApplication, m_pModelObject);
  else if (objectType == Renga::ObjectTypes::Room)
    return std::make_unique<RoomPropertyViewBuilder>(pPropertyManagers, m_pApplication, m_pModelObject);
  else
    return std::make_unique<PropertyViewBuilder>(pPropertyManagers, m_pApplication, m_pModelObject);
}

Renga::IParameterPtr PropertyViewModelObjectSource::getParameter(GUID parameterId)
{
  return m_pModelObject->GetParameters()->Get(parameterId);
}
