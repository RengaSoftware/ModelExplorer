//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtCore/QObject>

#include <RengaAPI/IInvokable.h>

class ModelExplorerButtonHandler : public QObject, public rengaapi::IInvokable
{
  Q_OBJECT

public:
  // rengaapi::IInvokable
  void invoke();

signals:
  void buttonClicked();
};