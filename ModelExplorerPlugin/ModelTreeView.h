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
#include <QtGui/QStandardItemModel>

#include "ObjectSelectionHandler.h"

#include <RengaAPI/ObjectId.h>

class ModelTreeView : public QTreeView
{
  Q_OBJECT
  friend class ModelExplorerWidget;
public:
  ModelTreeView(QTranslator* pTranslator, QWidget* pParent = nullptr);
  ~ModelTreeView();

signals:
  void modelObjectSelectionChanged(rengaapi::ObjectId objectId);

private slots:
  void onRebuildTree();
  void onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected);
  void onRengaObjectSelected(const rengaapi::ObjectId&);
  void showSelectedItem();
  void hideSelectedItem();
  void onTreeItemClicked(const QModelIndex& current);

private:
  void changeItemVisibility(bool show);
  void updateVisibilityIcon(const QModelIndex& iconIndex, const rengaapi::ObjectId& selectedObjectId);
  void changeChildrenVisibility(const QModelIndex& iconIndex, bool visible);
  void makeParentVisible(QStandardItem* childItem);

private:
  QTranslator* m_pTranslator;
  ObjectSelectionHandler m_objectSelectionHandler;
  std::unique_ptr<QStandardItemModel> m_pModel;
};
