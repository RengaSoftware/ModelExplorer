//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "LevelPropertyViewBuilder.h"
#include "PropertyManager.h"


LevelPropertyViewBuilder::LevelPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                                   Renga::IModelObjectPtr pModelObject) :
  PropertyViewBuilder(pApplication, pModelObject)
{
}

void LevelPropertyViewBuilder::createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  PropertyViewBuilder::createIntegratedParameters(mngr, propertyList);

  Renga::ILevelPtr pLevel;
  m_pModelObject->QueryInterface(&pLevel);

  mngr.addValue(propertyList, QApplication::translate("me_level", "name"), QString::fromWCharArray(pLevel->GetLevelName()));
  mngr.addValue(propertyList, QApplication::translate("me_level", "elevation"), pLevel->GetElevation());
}
