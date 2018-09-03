#pragma once

#include "IPropertyViewSourceObject.h"

class PropertyViewModelObjectSource : public IPropertyViewSourceObject
{
public:
  PropertyViewModelObjectSource(
    Renga::IApplicationPtr pApplication,
    Renga::IModelObjectPtr pModelObject);

  IObjectPropertyViewBuilder* createPropertyViewBuilder(PropertyManagers* pPropertyManagers) override;

  Renga::IPropertyPtr getUserDefinedProperty(GUID propertyId) override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IModelObjectPtr m_pModelObject;
};
