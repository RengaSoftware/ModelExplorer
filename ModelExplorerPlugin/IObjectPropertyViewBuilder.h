//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <qttreepropertybrowser.h>

#include <RengaAPI/ModelObject.h>

typedef std::list<QtProperty*> PropertyList;

class IObjectPropertyViewBuilder
{
public:
	virtual PropertyList createParametersProperties(rengaapi::ModelObject* pObject) = 0; 
	virtual PropertyList createQuantitiesProperties(rengaapi::ModelObject* pObject) = 0; 
	virtual PropertyList createUserAttributesProperties(rengaapi::ModelObject* pObject) = 0;
};