//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "RengaEventsHandler.h"

#include <RengaAPI/EventsDispatcher.h>

RengaEventsHandler::RengaEventsHandler()
{
  rengaapi::EventsDispatcher::registerEventHandler(this);
}

RengaEventsHandler::~RengaEventsHandler()
{
  rengaapi::EventsDispatcher::unregisterEventHandler(this);
}

void RengaEventsHandler::process(rengaapi::IEvent& event)
{
  if (event.type() == rengaapi::IEvent::Type::ProjectClose)
  {
    emit projectAboutToClose();
  }
}
