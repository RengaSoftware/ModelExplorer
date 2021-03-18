//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoomPropertyViewBuilder.h"
#include "PropertyManager.h"


RoomPropertyViewBuilder::RoomPropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                                 Renga::IModelObjectPtr pModelObject) :
  ObjectPropertyViewBuilder(pApplication, pModelObject)
{
}

void RoomPropertyViewBuilder::createIntegratedParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  ObjectPropertyViewBuilder::createIntegratedParameters(mngr, propertyList);

  Renga::IRoomPtr pRoom;
  m_pModelObject->QueryInterface(&pRoom);

  mngr.addValue(propertyList, QApplication::translate("me_room", "name"), QString::fromWCharArray(pRoom->GetRoomName()));
  mngr.addValue(propertyList, QApplication::translate("me_room", "roomNumber"), QString::fromWCharArray(pRoom->GetRoomNumber()));
}
