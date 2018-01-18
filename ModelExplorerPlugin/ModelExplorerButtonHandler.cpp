//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ModelExplorerButtonHandler.h"


ModelExplorerButtonHandler::ModelExplorerButtonHandler(Renga::IActionPtr pAction) :
  Renga::ActionEventHandler(pAction)
{
}

void ModelExplorerButtonHandler::OnTriggered()
{
  emit buttonClicked();
}

void ModelExplorerButtonHandler::OnToggled(bool checked)
{
}
