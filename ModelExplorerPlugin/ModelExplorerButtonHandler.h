//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtCore/QObject>

#include <Renga/ActionEventHandler.hpp>


class ModelExplorerButtonHandler : public QObject, public Renga::ActionEventHandler
{
  Q_OBJECT

public:
  ModelExplorerButtonHandler(Renga::IActionPtr pAction);

  void OnTriggered() override;
  void OnToggled(bool checked) override;

signals:
  void buttonClicked();
};