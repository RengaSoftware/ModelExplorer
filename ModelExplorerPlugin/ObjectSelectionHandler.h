//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtCore/QObject>

#include <Renga/SelectionEventHandler.hpp>


class ObjectSelectionHandler : public QObject, public Renga::SelectionEventHandler
{
  Q_OBJECT

public:
  ObjectSelectionHandler(Renga::ISelectionPtr pSelection);

  void OnModelSelectionChanged() override;

signals:
  void objectSelected(const int&);

private:
  Renga::ISelectionPtr m_pSelection;
};