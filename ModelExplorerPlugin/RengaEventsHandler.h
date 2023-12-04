//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtCore/QObject>

#include <Renga/ApplicationEventHandler.hpp>


class RengaEventsHandler : public QObject, public Renga::ApplicationEventHandler
{
  Q_OBJECT

public:
  RengaEventsHandler(Renga::IApplicationPtr pApplication);

  void OnBeforeApplicationClose(Renga::IApplicationCloseEvent* pEvent) override;
  void OnBeforeProjectClose(Renga::IProjectCloseEvent* pEvent) override;

signals:
  void projectAboutToClose();
};