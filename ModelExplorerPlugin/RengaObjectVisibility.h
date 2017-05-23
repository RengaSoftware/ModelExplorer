//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include <RengaAPI/Application.h>
#include <RengaAPI/LevelView.h>
#include <RengaAPI/ObjectId.h>
#include <RengaAPI/ObjectVisibility.h>
#include <RengaAPI/View.h>

typedef std::list<rengaapi::ObjectId> ObjectIdList;

static bool getRengaObjectVisibility(const rengaapi::ObjectId& objectId)
{
  rengaapi::View* pView = rengaapi::Application::activeView();
  switch(pView->type())
  {
  case rengaapi::ViewType::View3D:
    return rengaapi::ObjectVisibility::isVisibleIn3DView(objectId);
  case rengaapi::ViewType::Level:
    {
      rengaapi::ObjectId levelId = dynamic_cast<rengaapi::LevelView*>(pView)->levelId();
      return rengaapi::ObjectVisibility::isVisibleOnLevel(objectId, levelId);
    }
  default:
    return false;
  }
}

static void setRengaObjectVisibility(const ObjectIdList& objectIdList, const bool visible)
{
  rengaapi::ObjectIdCollection objectIdCollection;
  for (auto& id : objectIdList)
    objectIdCollection.add(id);

  rengaapi::View* pView = rengaapi::Application::activeView();
  switch(pView->type())
  {
  case rengaapi::ViewType::View3D:
    rengaapi::ObjectVisibility::setVisibleIn3DView(objectIdCollection, visible);
    break;
  case rengaapi::ViewType::Level:
    {
      rengaapi::ObjectId levelId = dynamic_cast<rengaapi::LevelView*>(pView)->levelId();
      rengaapi::ObjectVisibility::setVisibleOnLevel(objectIdCollection, levelId, visible);
      break;
    }
  default:
    break;
  }
}
