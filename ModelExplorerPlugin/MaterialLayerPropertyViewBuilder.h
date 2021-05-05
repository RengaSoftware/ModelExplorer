#pragma once

#include "PropertyViewBuilderBase.h"
#include "RengaModelUtils.h"


class MaterialLayerPropertyViewBuilder : public PropertyViewBuilderBase
{
public:
  MaterialLayerPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                   MaterialLayerAccess materialLayerAccess,
                                   LayerAccess layer);

  PropertyList createParameters(PropertyManager& mngr) override;
  PropertyList createQuantities(PropertyManager& mngr) override;
  PropertyList createProperties(PropertyManager& mng) override { return{}; };

private:
  Renga::IApplicationPtr m_pApplication;
  MaterialLayerAccess m_materialLayerAccess;
  LayerAccess m_layerAccess;
};
