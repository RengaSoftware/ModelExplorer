//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectPropertyViewBuilder.h"


class LevelPropertyViewBuilder : public ObjectPropertyViewBuilder
{
public:
  LevelPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                           Renga::IModelObjectPtr pModelObject);

  void createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList) override;
};
