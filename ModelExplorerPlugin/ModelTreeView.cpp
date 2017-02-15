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

#include <QtWidgets/QHeaderView.h>

#include <RengaAPI/ObjectIdCollection.h>
#include <RengaAPI/ObjectVisibility.h>

ModelTreeView::ModelTreeView(QTranslator* pTranslator, QWidget* pParent /*= nullptr*/)
  : QTreeView(pParent)
  , m_pTranslator(pTranslator)
{
  connect(&m_objectSelectionHandler, 
    SIGNAL(objectSelected(const rengaapi::ObjectId&)), 
    SLOT(onRengaObjectSelected(const rengaapi::ObjectId&)));

  connect(this, 
          SIGNAL(clicked(const QModelIndex&)), 
          this, 
          SLOT(onTreeItemClicked(const QModelIndex&)));
}

ModelTreeView::~ModelTreeView()
{}

void ModelTreeView::onRebuildTree()
{
  ModelTreeBuilder builder(m_pTranslator);

  m_pModel.reset(builder.buildModelTree());
  setModel(m_pModel.get());

  // set columns width & resize mode
  QHeaderView* treeHeader  = this->header();
  treeHeader->setStretchLastSection(false);
  treeHeader->setSectionResizeMode(treeHeader->logicalIndex(0), QHeaderView::ResizeMode::Stretch);
  treeHeader->setSectionResizeMode(treeHeader->logicalIndex(1), QHeaderView::ResizeMode::Fixed);
  treeHeader->resizeSection(1, 20);

  // check this
  QItemSelectionModel* pSelectionModel = selectionModel();
  connect(pSelectionModel, 
          SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
          SLOT(onTreeItemSelected(const QItemSelection&, const QItemSelection&)));


  // select first element in tree after rebuild
  pSelectionModel->select(m_pModel->index(0, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
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
      QModelIndex selectedObjectIndex = indexList.at(0);
      QVariant data = m_pModel->data(selectedObjectIndex, ModelTreeBuilder::objectIDRole);
      if (data.type() != QVariant::Invalid)
      {
        bool ok = false;
        unsigned int id = data.toUInt(&ok);
        if (ok)
        {
          selectedObjectId.setId(id);
          updateVisibilityIcon(m_pModel->index(selectedObjectIndex.row(), 1, selectedObjectIndex.parent()), selectedObjectId);
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
    pSelectionModel->setCurrentIndex(indexList.first(), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
    pSelectionModel->select(indexList.first(), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

    expand(indexList.first());
  }
}



void ModelTreeView::showSelectedItem()
{
  changeItemVisibility(true);
}

void ModelTreeView::hideSelectedItem()
{
  changeItemVisibility(false);
}

void ModelTreeView::onTreeItemClicked(const QModelIndex& iconIndex)
{
  if (iconIndex.column() == 0)
    return;

  const QModelIndex objectIndex = m_pModel->index(iconIndex.row(), 0, iconIndex.parent());
  QStandardItem* objectItem = m_pModel->itemFromIndex(objectIndex);

  bool isVisible;
  QVariant data = m_pModel->data(objectIndex, ModelTreeBuilder::objectIDRole);
  if (data.type() == QVariant::Invalid) // folder visibility
  {
    isVisible = m_pModel->itemFromIndex(iconIndex)->data().toBool();
    isVisible ^= true;
  }
  else
  {  
    bool ok = false;
    unsigned int id = data.toUInt(&ok);
    assert(ok);
    const rengaapi::ObjectId objectId(id);
    isVisible = !rengaapi::ObjectVisibility::isVisibleIn3DView(objectId); // TODO: RengaSDK returns inverted visibility value, it's a bug!
    isVisible ^= true;
  }

  if (isVisible)
  {
    // check if object's level was hidden
    if (objectItem->parent() != nullptr)
    {
      // get level id
      QStandardItem* levelItem = (data.type() == QVariant::Invalid ? objectItem->parent() : objectItem->parent()->parent());
      QVariant levelData = m_pModel->data(levelItem->index(), ModelTreeBuilder::objectIDRole);
      bool ok = false;
      unsigned int levelId = levelData.toUInt(&ok);
      assert(ok);

      // show level if necessary
      if (rengaapi::ObjectVisibility::isVisibleIn3DView(levelId)) // TODO: if (not...) after bugfix
      {
        rengaapi::ObjectIdCollection objectIdCollection;
        objectIdCollection.add(rengaapi::ObjectId(levelId));
        rengaapi::ObjectVisibility::setVisibleIn3DView(objectIdCollection, isVisible);
        changeChildrenVisibility(levelItem->index(), isVisible);
      }
    }
  }

  if (data.type() == QVariant::Invalid)
  {
    const QModelIndex objectIndex = m_pModel->index(iconIndex.row(), 0, iconIndex.parent());
    QStandardItem* objectItem = m_pModel->itemFromIndex(objectIndex);
    for (size_t i = 0; i < objectItem->rowCount(); ++i)
    {
      QStandardItem* child = objectItem->child(i);
      bool ok = false;
      unsigned int id = child->data().toUInt(&ok);
      assert(ok);
      // show children in folder
      rengaapi::ObjectIdCollection objectIdCollection;
      objectIdCollection.add(rengaapi::ObjectId(id));
      rengaapi::ObjectVisibility::setVisibleIn3DView(objectIdCollection, isVisible);
    }
  }
  else
  {
    // show object
    rengaapi::ObjectIdCollection objectIdCollection;
    objectIdCollection.add(data.toUInt());
    rengaapi::ObjectVisibility::setVisibleIn3DView(objectIdCollection, isVisible);

    // in some cases objects don't change visibility, get current state
    isVisible = !rengaapi::ObjectVisibility::isVisibleIn3DView(data.toUInt());
    m_pModel->itemFromIndex(iconIndex)->setData(QVariant(isVisible));
  }

  // hide/show all children
  changeChildrenVisibility(iconIndex, isVisible);

  // Åñëè îáúåêò ñòàë âèäåí, òî íàäî ïîêàçàòü âñå óçëû ïî ïóòè äî ðîäèòåëÿ
  if (isVisible)
    makeParentVisible(objectItem);
}

void ModelTreeView::changeItemVisibility(bool show)
{
  QModelIndexList list = selectedIndexes();
  uint len = list.length();
  if (len == 0)
    return;

  QModelIndex itemIndex = list.first();
  QModelIndex iconIndex = list.at(1);
  QStandardItem* selectedItem = m_pModel->itemFromIndex(itemIndex);
  QVariant data = selectedItem->data();
  if (data.isValid())
  {
    bool ok = false;
    uint id = data.toUInt(&ok);
    assert(ok);
    updateVisibilityIcon(iconIndex, rengaapi::ObjectId(id));
  }

  bool isVisible = m_pModel->itemFromIndex(iconIndex)->data().toBool();
  if (isVisible ^ show)
    onTreeItemClicked(iconIndex);
}

void ModelTreeView::updateVisibilityIcon(const QModelIndex& iconIndex, const rengaapi::ObjectId& selectedObjectId)
{
  QStandardItem* selectedObjectIcon = m_pModel->itemFromIndex(iconIndex);
  bool isVisible = !rengaapi::ObjectVisibility::isVisibleIn3DView(selectedObjectId);
  selectedObjectIcon->setIcon(QIcon(isVisible ? ":/icons/Visible" : ":/icons/Hidden"));
  selectedObjectIcon->setData(QVariant(isVisible));
}

void ModelTreeView::changeChildrenVisibility(const QModelIndex& iconIndex, bool visible)
{
  // change visibility of current item
  QStandardItem* iconItem = m_pModel->itemFromIndex(iconIndex);
  iconItem->setIcon(QIcon(visible ? ":/icons/Visible" : ":/icons/Hidden"));
  iconItem->setData(QVariant(visible));

  // visit all children
  const QModelIndex objectIndex = m_pModel->index(iconIndex.row(), 0, iconIndex.parent());
  QStandardItem* objectItem = m_pModel->itemFromIndex(objectIndex);
  for (size_t i = 0; i < objectItem->rowCount(); ++i)
  {
    QModelIndex childObjectIndex = objectItem->child(i)->index();
    QModelIndex childIconIndex = m_pModel->index(childObjectIndex.row(), 1, objectIndex);
    changeChildrenVisibility(childIconIndex, visible);
  }
}

void ModelTreeView::makeParentVisible(QStandardItem* childItem)
{
  QStandardItem* parent = childItem->parent();
  if (parent != nullptr)
  {
    const QModelIndex parentIndex = parent->index();
    QStandardItem* parentIcon = m_pModel->itemFromIndex(m_pModel->index(parentIndex.row(), 1, parentIndex.parent()));
    parentIcon->setIcon(QIcon(":/icons/Visible"));
    parentIcon->setData(QVariant(true));
    makeParentVisible(parent);
  }
}