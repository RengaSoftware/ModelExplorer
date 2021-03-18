#pragma once

#include "IPropertyViewBuilder.h"


class PropertyViewBuilderBase : public IPropertyViewBuilder
{
protected:
  PropertyList createPropertiesInternal(PropertyManager& mng, Renga::IPropertyContainer& container);
  PropertyList createQuantitiesInternal(PropertyManager& mng, Renga::IQuantityContainer& container);
};