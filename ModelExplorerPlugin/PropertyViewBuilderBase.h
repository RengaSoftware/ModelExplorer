#pragma once

#include "IPropertyViewBuilder.h"


class PropertyViewBuilderBase : public IPropertyViewBuilder
{
protected:
  PropertyList createQuantitiesInternal(PropertyManager& mng, Renga::IQuantityContainer& container);
};