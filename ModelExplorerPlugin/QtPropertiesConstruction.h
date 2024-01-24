//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "RengaModelUtils.h"


class PropertyManager;

void qtPropertiesFromRengaQuantities(PropertyManager& mng, Renga::IQuantityContainer& container);
void qtPropertiesFromRengaParameters(
    PropertyManager& mng,
    Renga::IParameterContainer& container,
    EntityByParameterIdNameGetter nameGetter);
void qtPropertiesFromRengaProperties(
    PropertyManager& mng,
    Renga::IPropertyManager& rengaPropertyManager,
    Renga::IPropertyContainer& container);