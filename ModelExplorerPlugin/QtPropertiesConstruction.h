//
// Copyright "Renga Software" LLC, 2016. All rights reserved.
//
// "Renga Software" LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// "Renga Software" LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

class PropertyManager;

void qtPropertiesFromRengaQuantities(PropertyManager& mng, Renga::IQuantityContainer& container);
void qtPropertiesFromRengaParameters(PropertyManager& mng, Renga::IParameterContainer& container);
void qtPropertiesFromRengaProperties(PropertyManager& mng, Renga::IPropertyContainer& container);