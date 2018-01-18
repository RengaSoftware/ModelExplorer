//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoomPropertyViewBuilder.h"


RoomPropertyViewBuilder::RoomPropertyViewBuilder(const PropertyManagers* pPropertyManagers, Renga::IApplicationPtr pApplication) :
  ObjectPropertyViewBuilder(pPropertyManagers, pApplication)
{
}

void RoomPropertyViewBuilder::createParametersProperties(PropertyList& propertyList, Renga::IModelObjectPtr pObject)
{
  ObjectPropertyViewBuilder::createParametersProperties(propertyList, pObject);

  Renga::IRoomPtr pRoom;
  pObject->QueryInterface(&pRoom);

  addValue(propertyList, QApplication::translate("me_room", "name"), QString::fromWCharArray(pRoom->GetRoomName()));
  addValue(propertyList, QApplication::translate("me_room", "roomNumber"), QString::fromWCharArray(pRoom->GetRoomNumber()));
}
