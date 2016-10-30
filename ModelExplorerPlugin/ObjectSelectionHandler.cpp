//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectSelectionHandler.h"

#include <RengaAPI/ModelSelection.h>

ObjectSelectionHandler::ObjectSelectionHandler()
{
  rengaapi::ModelSelection::addChangesHandler(this);
}

ObjectSelectionHandler::~ObjectSelectionHandler()
{
  rengaapi::ModelSelection::removeChangesHandler(this);
}

void ObjectSelectionHandler::invoke()
{
  rengaapi::ObjectIdCollection selectedObjCollection = rengaapi::ModelSelection::objectIds();
  if (selectedObjCollection.size() == 0)
    return;

  auto firstSelectedObjectId = selectedObjCollection.get(0).id();
  emit objectSelected(firstSelectedObjectId);
}
