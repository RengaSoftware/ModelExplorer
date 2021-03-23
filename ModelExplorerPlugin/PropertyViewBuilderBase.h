#pragma once

#include "IPropertyViewBuilder.h"


class PropertyViewBuilderBase : public IPropertyViewBuilder
{
protected:
  PropertyViewBuilderBase(bool disableProperties = true);

  PropertyList createParametersInternal(PropertyManager& mng, Renga::IParameterContainer& container);
  PropertyList createPropertiesInternal(PropertyManager& mng, Renga::IPropertyContainer& container);
  PropertyList createQuantitiesInternal(PropertyManager& mng, Renga::IQuantityContainer& container);

  bool m_disableProperties = true;
};