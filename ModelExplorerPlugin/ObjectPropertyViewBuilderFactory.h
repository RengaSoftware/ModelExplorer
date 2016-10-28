//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

class ObjectPropertyViewBuilderFactory
{
public:
  ObjectPropertyViewBuilderFactory(const PropertyManagers* pPropertyManagers, const QTranslator* pTranslator);
	ObjectPropertyViewBuilder* createBuilder(rengaapi::ModelObject* pModelObject);

private:
  const PropertyManagers* m_pPropertyManagers;
  const QTranslator* m_pTranslator;
};