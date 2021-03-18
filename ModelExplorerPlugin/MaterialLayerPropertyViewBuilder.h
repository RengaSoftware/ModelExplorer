#pragma once

#include "PropertyViewBuilderBase.h"


class MaterialLayerPropertyViewBuilder : public PropertyViewBuilderBase
{
public:
  MaterialLayerPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                   Renga::IMaterialLayerPtr pMaterialLayer,
                                   Renga::ILayerPtr pLayer);

  void createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IMaterialLayerPtr m_pMaterialLayer;
  Renga::ILayerPtr m_pLayer;
};
