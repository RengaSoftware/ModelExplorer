#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class MaterialLayerPropertyViewBuilder : public CPropertyViewBuilder
{
public:
  MaterialLayerPropertyViewBuilder(const PropertyManagers* pPropertyManagers,
                                   Renga::IApplicationPtr pApplication,
                                   Renga::IMaterialLayerPtr pMaterialLayer,
                                   Renga::ILayerPtr pLayer);

  void createIntegratedParameters(PropertyList& propertyList) override;
  void createQuantities(PropertyList& propertyList) override;

private:
  const PropertyManagers* m_pPropertyManagers;
  Renga::IApplicationPtr m_pApplication;
  Renga::IMaterialLayerPtr m_pMaterialLayer;
  Renga::ILayerPtr m_pLayer;
};
