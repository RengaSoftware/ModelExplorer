//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectPropertyViewBuilder.h"

class RailingPropertyViewBuilder : public ObjectPropertyViewBuilder
{
public:
	RailingPropertyViewBuilder(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator);

	PropertyMap createParametersProperties(rengaapi::ModelObject* pObject); 
	PropertyMap createQuantitiesProperties(rengaapi::ModelObject* pObject); 
};