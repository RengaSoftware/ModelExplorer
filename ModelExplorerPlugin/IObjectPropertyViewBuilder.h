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

typedef std::map<QString, QtProperty*> PropertyMap;

class IObjectPropertyViewBuilder
{
public:
	virtual PropertyMap createParametersProperties(rengaapi::ModelObject* pObject) = 0; 
	virtual PropertyMap createQuantitiesProperties(rengaapi::ModelObject* pObject) = 0; 
	virtual PropertyMap createUserAttributesProperties(rengaapi::ModelObject* pObject) = 0;
};