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
  void updateVisibilityIcon(const QModelIndex& itemIndex, const QModelIndex& iconIndex);
  void changeVisibilityWithChildren(const QModelIndex& iconIndex, bool visible);
  void showParentItems(QStandardItem* childItem);

  bool isModelObjectVisible(const QVariant& data);
  void setModelObjectVisibility(const QVariant& data, const bool visible);
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
  QTranslator* m_pTranslator;
  ObjectSelectionHandler m_objectSelectionHandler;
};