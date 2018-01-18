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
#include "BoolGuard.h"

#include <QtWidgets/QHeaderView.h>

#include <atlsafe.h>


const uint c_iconColumnSize = 30;
const QItemSelectionModel::SelectionFlags c_selectRows = QItemSelectionModel::SelectionFlag::Select | QItemSelectionModel::Rows;
const QItemSelectionModel::SelectionFlags c_selectCurrentRows = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;

ModelTreeView::ModelTreeView(QWidget* pParent /*= nullptr*/, Renga::IApplicationPtr pApplication) :
  QTreeView(pParent),
  m_pApplication(pApplication),
  m_wasObjectSelectedInModel(false)
{
  m_objectSelectionHandler.reset(new ObjectSelectionHandler(m_pApplication->GetSelection()));

  connect(m_objectSelectionHandler.get(), 
    SIGNAL(objectSelected(const int&)), 
    SLOT(onRengaObjectSelected(const int&)));

  connect(this, 
    SIGNAL(clicked(const QModelIndex&)), 
    this, 
    SLOT(onTreeItemClicked(const QModelIndex&)));
}

ModelTreeView::~ModelTreeView()
{}

void ModelTreeView::onRebuildTree()
{
  ModelTreeBuilder builder(m_pApplication);
  m_pModel.reset(builder.buildModelTree());
  setModel(m_pModel.get());

  // set columns width & resize mode
  QHeaderView* treeHeader  = this->header();
  treeHeader->setStretchLastSection(false);
  treeHeader->setSectionResizeMode(treeHeader->logicalIndex(0), QHeaderView::ResizeMode::Stretch);
  treeHeader->setSectionResizeMode(treeHeader->logicalIndex(1), QHeaderView::ResizeMode::Fixed);
  treeHeader->resizeSection(1, c_iconColumnSize);

  // check this
  QItemSelectionModel* pSelectionModel = selectionModel();
  connect(pSelectionModel, 
    SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
    SLOT(onTreeItemSelected(const QItemSelection&, const QItemSelection&)));

  // select first element in tree after rebuild
  pSelectionModel->select(getModel()->index(0, 0), c_selectRows);
}

void ModelTreeView::onTreeItemSelected(const QItemSelection& selected, const QItemSelection& deselected)
{
  int selectedObjectId(0);
  if (!selected.empty())
  {
    QModelIndexList indexList = selected.indexes();
    if (!indexList.empty())
    {
      // get selected object id from tree view
      QModelIndex selectedObjectIndex = indexList.at(0);
      QVariant data = getModel()->data(selectedObjectIndex, ModelTreeBuilder::objectIDRole);
      if (data.type() != QVariant::Invalid)
      {
        bool ok = false;
        unsigned int id = data.toUInt(&ok);
        if (ok)
        {
          selectedObjectId = id;

          const QModelIndex selectedIconIndex = getModel()->index(selectedObjectIndex.row(), 1, selectedObjectIndex.parent());
          updateVisibilityIcon(selectedObjectIndex, selectedIconIndex);

          if (!m_wasObjectSelectedInModel)
          {
            CComSafeArray<int> idsSafeArray(static_cast<ULONG>(1));
            idsSafeArray.SetAt(0, id);

            auto pSelection = m_pApplication->GetSelection();
            pSelection->SetSelectedModelObjects(idsSafeArray);
          }
        }
      }
    }
  }
  emit modelObjectSelectionChanged(selectedObjectId);
}

void ModelTreeView::onRengaObjectSelected(const int& objectId)
{
  QAbstractItemModel* pModel = model();
  assert(pModel != nullptr);

  QModelIndexList indexList = pModel->match(pModel->index(0,0), 
    ModelTreeBuilder::objectIDRole, 
    objectId, 
    1,
    Qt::MatchRecursive);

  if (!indexList.empty())
  {
    QItemSelectionModel* pSelectionModel = selectionModel();

    BoolGuard guard(m_wasObjectSelectedInModel, true);

    pSelectionModel->setCurrentIndex(indexList.first(), c_selectCurrentRows);
    pSelectionModel->select(indexList.first(), c_selectCurrentRows);

    expand(indexList.first());
  }
}

void ModelTreeView::onTreeItemClicked(const QModelIndex& iconIndex)
{
  if (iconIndex.column() == 0)
    return;

  auto pModel = getModel();
  const QModelIndex itemIndex = pModel->index(iconIndex.row(), 0, iconIndex.parent());
  QStandardItem* item = pModel->itemFromIndex(itemIndex);
  QStandardItem* iconItem = pModel->itemFromIndex(iconIndex);

  bool isVisible;
  QVariant data = item->data();
  if (isModelObject(data))
  {
    isVisible = isModelObjectVisible(data);
  }
  else
  {
    // get folder visibility
    updateVisibilityIcon(itemIndex, iconIndex);
    isVisible = iconItem->data().toBool();
  }
  isVisible ^= true;


  // show parent items only if current item visible
  // Note: when you show level, all objects on level will be shown. It's renga bug ¹21908
  if (isVisible)
    setRengaObjectVisibility(m_pApplication, getParentObjectIdList(item), true);

  // hide/show all children
  setRengaObjectVisibility(m_pApplication, getObjectIdListWithChildren(iconIndex, isVisible), isVisible);
}

void ModelTreeView::showSelectedItem()
{
  changeItemVisibility(true);
}

void ModelTreeView::hideSelectedItem()
{
  changeItemVisibility(false);
}

void ModelTreeView::changeItemVisibility(bool show)
{
  QModelIndexList list = selectedIndexes();
  uint len = list.length();
  if (len == 0)
    return;

  assert(len == 2);
  QModelIndex itemIndex = list.at(0);
  QModelIndex iconIndex = list.at(1);
  assert(itemIndex.row() == iconIndex.row());

  updateVisibilityIcon(itemIndex, iconIndex);

  if (show)
    setRengaObjectVisibility(m_pApplication, getParentObjectIdList(getModel()->itemFromIndex(itemIndex)), true);

  setRengaObjectVisibility(m_pApplication, getObjectIdListWithChildren(iconIndex, show), show);
}

void ModelTreeView::updateVisibilityIcon(const QModelIndex& itemIndex, const QModelIndex& iconIndex)
{
  QStandardItem* selectedItem = getModel()->itemFromIndex(itemIndex);
  bool isVisible = false;
  QVariant data = selectedItem->data();
  if (isModelObject(data))
  {
    // get actual ModelObject visibility
    isVisible = isModelObjectVisible(data);
  }
  else
  {
    // get folder visibility
    if (selectedItem->rowCount() > 0)
    {
      // look for children items
      for (size_t i = 0; i < selectedItem->rowCount(); ++i)
      {
        QStandardItem* childItem = selectedItem->child(i);
        data = childItem->data();
        if (isModelObjectVisible(data))
        {
          isVisible = true;
          break;
        }
      }
    }
    else
    {
      // get level visibility
      QStandardItem* parent = selectedItem->parent();
      data = parent->data();
      isVisible = isModelObjectVisible(data);
    }
  }

  setIcon(iconIndex, isVisible);
}

ObjectIdList ModelTreeView::getObjectIdListWithChildren(const QModelIndex& iconIndex, bool visible)
{
  ObjectIdList result;

  // add current object id if necessary
  const QModelIndex itemIndex = getModel()->index(iconIndex.row(), 0, iconIndex.parent());
  QStandardItem* item = getModel()->itemFromIndex(itemIndex);
  QVariant data = item->data();
  if (isModelObject(data))
    result.push_back(getRengaObjectIdFromData(data));

  if (item->rowCount() > 0)
  {
    // visit all children
    bool hasVisibleChild = false;
    for (size_t i = 0; i < item->rowCount(); ++i)
    {
      QModelIndex childItemIndex = item->child(i)->index();
      QModelIndex childIconIndex = getModel()->index(childItemIndex.row(), 1, itemIndex);
      result.splice(result.end(), getObjectIdListWithChildren(childIconIndex, visible));
      hasVisibleChild |= getModel()->itemFromIndex(childIconIndex)->data().toBool();
    }
    visible = hasVisibleChild;
  }

  // change visibility of current item in tree
  setIcon(iconIndex, visible);

  return result;
}

ObjectIdList ModelTreeView::getParentObjectIdList(QStandardItem* child)
{
  ObjectIdList result;

  QStandardItem* parent = child->parent();
  if (parent == nullptr)
    return result;

  // make icon visible
  QModelIndex parentIndex = parent->index();
  QModelIndex parentIconIndex = getModel()->index(parentIndex.row(), 1, parentIndex.parent());
  setIcon(parentIconIndex, true);

  // add object id if necessary
  QVariant parentData = parent->data();
  if (isModelObject(parentData) && !isModelObjectVisible(parentData))
    result.push_back(getRengaObjectIdFromData(parentData));

  result.splice(result.end(), getParentObjectIdList(parent));

  return result;
}

bool ModelTreeView::isModelObjectVisible(const QVariant& data)
{
  const auto objectId = getRengaObjectIdFromData(data);
  return getRengaObjectVisibility(m_pApplication, objectId);
}

int ModelTreeView::getRengaObjectIdFromData(const QVariant& data) const
{
  bool ok = false;
  uint id = data.toUInt(&ok);
  assert(ok);
  return id;
}

void ModelTreeView::setIcon(const QModelIndex& iconIndex, bool visible)
{
  QStandardItem* iconItem = getModel()->itemFromIndex(iconIndex);
  iconItem->setIcon(QIcon(visible ? ":/icons/Visible" : ":/icons/Hidden"));
  iconItem->setData(QVariant(visible));
}
