//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RengaEventsHandler.h"


RengaEventsHandler::RengaEventsHandler(Renga::IApplicationPtr pApplication) :
  Renga::ApplicationEventHandler(pApplication)
{
}

void RengaEventsHandler::OnBeforeApplicationClose(Renga::IApplicationCloseEvent* pEvent)
{
}

void RengaEventsHandler::OnBeforeProjectClose(Renga::IProjectCloseEvent* pEvent)
{
  emit projectAboutToClose();
}
