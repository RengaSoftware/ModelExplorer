//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtCore/QObject>

#include <RengaAPI/IEventHandler.h>

class RengaEventsHandler : public QObject, public rengaapi::IEventHandler
{
  Q_OBJECT

public:
  RengaEventsHandler();
  ~RengaEventsHandler();

  //rengaapi::IEventHandler
  void process(rengaapi::IEvent& event);

signals:
  void projectAboutToClose();
};