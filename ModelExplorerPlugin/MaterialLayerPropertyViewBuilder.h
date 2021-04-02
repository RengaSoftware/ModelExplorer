#pragma once

#include "PropertyViewBuilderBase.h"
#include "RengaModelUtils.h"


class MaterialLayerPropertyViewBuilder : public PropertyViewBuilderBase
{
public:
  MaterialLayerPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                   MaterialLayerAccess materialLayerAccess,
                                   LayerAccess layer);

  void createParameters(PropertyManager& mngr, PropertyList& propertyList) override;
  void createQuantities(PropertyManager& mngr, PropertyList& propertyList) override;

private:
  Renga::IApplicationPtr m_pApplication;
  MaterialLayerAccess m_materialLayerAccess;
  LayerAccess m_layerAccess;
};
