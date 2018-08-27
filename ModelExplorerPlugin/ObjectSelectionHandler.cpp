//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectSelectionHandler.h"

#include <atlsafe.h>


ObjectSelectionHandler::ObjectSelectionHandler(Renga::ISelectionPtr pSelection) :
  Renga::SelectionEventHandler(pSelection),
  m_pSelection(pSelection)
{
}

void ObjectSelectionHandler::OnModelSelectionChanged()
{
  CComSafeArray<int> selectedObjectIds(m_pSelection->GetSelectedObjects());

  if (selectedObjectIds.GetCount() == 0)
    return;

  auto firstSelectedObjectId = selectedObjectIds.GetAt(0);
  emit objectSelected(firstSelectedObjectId);
}
