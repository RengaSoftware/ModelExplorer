//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtWidgets/QTreeView>
#include <QtGui/QStandardItemModel>

#include "ObjectSelectionHandler.h"
#include "RengaObjectVisibility.h"

#include <RengaAPI/ObjectId.h>

class ModelTreeView : public QTreeView
{
  Q_OBJECT

public:
  ModelTreeView(QWidget* pParent = nullptr);
  ~ModelTreeView();

signals:
  void modelObjectSelectionChanged(rengaapi::ObjectId objectId);

private slots:
  void onRebuildTree();
  void onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected);
  void onRengaObjectSelected(const rengaapi::ObjectId&);
  void onTreeItemClicked(const QModelIndex& current);
  void showSelectedItem();
  void hideSelectedItem();

private:
  void changeItemVisibility(bool show);
  void updateVisibilityIcon(const QModelIndex& itemIndex, const QModelIndex& iconIndex);
  ObjectIdList getObjectIdListWithChildren(const QModelIndex& iconIndex, bool visible);
  ObjectIdList getParentObjectIdList(QStandardItem* childItem);

  bool isModelObjectVisible(const QVariant& data);
  rengaapi::ObjectId getRengaObjectIdFromData(const QVariant& data) const;
  void setIcon(const QModelIndex& iconIndex, bool visible);

  inline QStandardItemModel* getModel()
  {
    return dynamic_cast<QStandardItemModel*>(model());
  }

  inline bool isModelObject(const QVariant& data)
  {
    return !data.isNull();
  }

private:
  ObjectSelectionHandler m_objectSelectionHandler;
  std::unique_ptr<QStandardItemModel> m_pModel;
  bool m_wasObjectSelectedInModel;
};