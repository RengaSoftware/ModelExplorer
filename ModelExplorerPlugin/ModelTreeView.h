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


class ModelTreeView : public QTreeView
{
  Q_OBJECT

public:
  ModelTreeView(QWidget* pParent, Renga::IApplicationPtr pApplication);
  ~ModelTreeView();

signals:
  void modelObjectSelectionChanged(int objectId);

private slots:
  void onRebuildTree();
  void onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected);
  void onRengaObjectSelected(const int&);
  void onTreeItemClicked(const QModelIndex& current);
  void showSelectedItem();
  void hideSelectedItem();

private:
  void changeItemVisibility(bool show);
  void updateVisibilityIcon(const QModelIndex& itemIndex, const QModelIndex& iconIndex);
  ObjectIdList getObjectIdListWithChildren(const QModelIndex& iconIndex, bool visible);
  ObjectIdList getParentObjectIdList(QStandardItem* childItem);

  bool isModelObjectVisible(const QVariant& data);
  int getRengaObjectIdFromData(const QVariant& data) const;
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
  Renga::IApplicationPtr m_pApplication;
  std::unique_ptr<ObjectSelectionHandler> m_objectSelectionHandler;
  std::unique_ptr<QStandardItemModel> m_pModel;
  bool m_wasObjectSelectedInModel;
};