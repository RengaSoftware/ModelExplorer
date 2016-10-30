//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ModelTreeView.h"
#include "ModelTreeBuilder.h"

#include <QtGui/QStandardItemModel>

ModelTreeView::ModelTreeView(QTranslator* pTranslator, QWidget* pParent /*= nullptr*/)
  : QTreeView(pParent)
  , m_pTranslator(pTranslator)
{
  connect(&m_objectSelectionHandler, 
    SIGNAL(objectSelected(const rengaapi::ObjectId&)), 
    SLOT(onRengaObjectSelected(const rengaapi::ObjectId&)));
}

ModelTreeView::~ModelTreeView()
{}

void ModelTreeView::onRebuildTree()
{
  ModelTreeBuilder builder(m_pTranslator);
  QStandardItemModel* pModel = builder.buildModelTree();
  setModel(pModel);

  QItemSelectionModel* pSelectionModel = selectionModel();
  connect(pSelectionModel, 
          SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
          SLOT(onTreeItemSelected(const QItemSelection&, const QItemSelection&)));

  // select first element in tree after rebuild
  pSelectionModel->select(pModel->index(0, 0), QItemSelectionModel::Select);
}

void ModelTreeView::onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected)
{
  rengaapi::ObjectId selectedObjectId(0);
  if (!selected.empty())
  {
    QModelIndexList indexList = selected.indexes();
    if (!indexList.empty())
    {
      // get selected object id from tree view
      QVariant data = model()->data(indexList.at(0), ModelTreeBuilder::objectIDRole);
      if (data.type() != QVariant::Invalid)
      {
        bool ok = false;
        unsigned int id = data.toUInt(&ok);
        if (ok)
        {
          selectedObjectId.setId(id);
        }
      }
    }
  }
  emit modelObjectSelectionChanged(selectedObjectId);
}

void ModelTreeView::onRengaObjectSelected(const rengaapi::ObjectId& objectId)
{
  QAbstractItemModel* pModel = model();
  assert(pModel != nullptr);

  QModelIndexList indexList = pModel->match(pModel->index(0,0), 
    ModelTreeBuilder::objectIDRole, 
    objectId.id(), 
    1,
    Qt::MatchRecursive);

  if (!indexList.empty())
  {
    QItemSelectionModel* pSelectionModel = selectionModel();
    pSelectionModel->setCurrentIndex(indexList.first(), QItemSelectionModel::SelectCurrent);
    pSelectionModel->select(indexList.first(), QItemSelectionModel::SelectCurrent);

    expand(indexList.first());
  }
}
