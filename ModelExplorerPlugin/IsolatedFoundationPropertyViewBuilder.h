//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectPropertyViewBuilder.h"

class IsolatedFoundationPropertyViewBuilder : public ObjectPropertyViewBuilder
{
public:
	IsolatedFoundationPropertyViewBuilder(const PropertyManagers* pPropertyManagers);

	PropertyList createParametersProperties(rengaapi::ModelObject* pObject); 
	PropertyList createQuantitiesProperties(rengaapi::ModelObject* pObject); 
};