//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "IObjectPropertyViewBuilder.h"
#include "PropertyManagers.h"

class ObjectPropertyViewBuilderFactory
{
public:
  ObjectPropertyViewBuilderFactory(const PropertyManagers* pPropertyManagers, Renga::IApplicationPtr pApplication);
  
  IObjectPropertyViewBuilder* createBuilderForModelObject(Renga::IModelObjectPtr pModelObject);
  IObjectPropertyViewBuilder* createBuilderForMaterialLayer(Renga::IMaterialLayerPtr pMaterialLayer, Renga::ILayerPtr pLayer);
  IObjectPropertyViewBuilder* createBuilderForRebarUsage(Renga::IRebarUsagePtr pRebarUsage);
  IObjectPropertyViewBuilder* createBuilderForReinforcementUnitUsage(Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

private:
  Renga::IApplicationPtr m_pApplication;
  const PropertyManagers* m_pPropertyManagers;
};
