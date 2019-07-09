//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoomPropertyViewBuilder.h"


RoomPropertyViewBuilder::RoomPropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                                 Renga::IApplicationPtr pApplication,
                                                 Renga::IModelObjectPtr pModelObject) :
  PropertyViewBuilder(pPropertyManagers, pApplication, pModelObject)
{
}

void RoomPropertyViewBuilder::createIntegratedParameters(PropertyList& propertyList)
{
  PropertyViewBuilder::createIntegratedParameters(propertyList);

  Renga::IRoomPtr pRoom;
  m_pModelObject->QueryInterface(&pRoom);

  auto& mngr = m_pPropertyManagers->m_default;
  mngr.addValue(propertyList, QApplication::translate("me_room", "name"), QString::fromWCharArray(pRoom->GetRoomName()));
  mngr.addValue(propertyList, QApplication::translate("me_room", "roomNumber"), QString::fromWCharArray(pRoom->GetRoomNumber()));
}
