#pragma once

#include "IPropertyViewBuilder.h"
#include "RengaModelUtils.h"


class MaterialLayerPropertyViewBuilder : public IPropertyViewBuilder
{
public:
  MaterialLayerPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                   MaterialLayerAccess materialLayerAccess,
                                   LayerAccess layer);

  void createParameters(PropertyManager& mngr) const override;
  void createQuantities(PropertyManager& mngr) const override;
  void createProperties(PropertyManager& mng) const override {}

private:
  Renga::IApplicationPtr m_pApplication;
  MaterialLayerAccess m_materialLayerAccess;
  LayerAccess m_layerAccess;
};
