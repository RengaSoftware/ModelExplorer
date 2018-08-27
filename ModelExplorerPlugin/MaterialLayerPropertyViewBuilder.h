#pragma once

#include "IObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

class MaterialLayerPropertyViewBuilder : public IObjectPropertyViewBuilder
{
public:
  MaterialLayerPropertyViewBuilder(
    const PropertyManagers* pPropertyManagers,
    Renga::IApplicationPtr pApplication,
    Renga::IMaterialLayerPtr pMaterialLayer,
    Renga::ILayerPtr pLayer);

  void createParametersProperties(PropertyList& propertyList) override;
  void createQuantitiesProperties(PropertyList& propertyList) override;
  PropertyList createUserAttributesProperties() override;

private:
  const PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IMaterialLayerPtr m_pMaterialLayer;
  Renga::ILayerPtr m_pLayer;
};
