//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtWidgets/QTreeView>
#include <QtCore/QTranslator.h>

#include "ObjectSelectionHandler.h"

#include <RengaAPI/ObjectId.h>

class ModelTreeView : public QTreeView
{
  Q_OBJECT

public:
  ModelTreeView(QTranslator* pTranslator, QWidget* pParent = nullptr);
  ~ModelTreeView();

signals:
  void modelObjectSelectionChanged(rengaapi::ObjectId objectId);

private slots:
  void onRebuildTree();
  void onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected);
  void onRengaObjectSelected(const rengaapi::ObjectId&);

private:
  QTranslator* m_pTranslator;
  ObjectSelectionHandler m_objectSelectionHandler;
};
