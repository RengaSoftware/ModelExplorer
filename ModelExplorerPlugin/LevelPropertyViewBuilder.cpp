//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "LevelPropertyViewBuilder.h"


LevelPropertyViewBuilder::LevelPropertyViewBuilder(const PropertyManagers* pPropertyManagers, Renga::IApplicationPtr pApplication) :
  ObjectPropertyViewBuilder(pPropertyManagers, pApplication)
{
}

void LevelPropertyViewBuilder::createParametersProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject)
{
  ObjectPropertyViewBuilder::createParametersProperties(propertyList, pObject);

  Renga::ILevelPtr pLevel;
  pObject->QueryInterface(&pLevel);

  addValue(propertyList, QApplication::translate("me_level", "name"), QString::fromWCharArray(pLevel->GetLevelName()));
  addValue(propertyList, QApplication::translate("me_level", "elevation"), pLevel->GetElevation());
}
