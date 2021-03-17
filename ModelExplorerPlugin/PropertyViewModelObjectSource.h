#pragma once

#include "IPropertyViewSourceObject.h"

class PropertyViewModelObjectSource : public IPropertyViewSourceObject
{
public:
  PropertyViewModelObjectSource(Renga::IApplicationPtr pApplication,
                                Renga::IModelObjectPtr pModelObject);

  std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder(PropertyManagers* pPropertyManagers) override;

  Renga::IParameterPtr getParameter(GUID parameterId) override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IModelObjectPtr m_pModelObject;
};
