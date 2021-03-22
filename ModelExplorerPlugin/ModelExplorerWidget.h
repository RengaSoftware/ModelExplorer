//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyView.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtGui/QStandardItemModel>

#include "ObjectSelectionHandler.h"
#include "RengaObjectVisibility.h"

namespace Ui
{
  class ModelExplorerDialog;
}

class ModelExplorerWidget : public QWidget
{
  Q_OBJECT

public:
  ModelExplorerWidget(Renga::IApplicationPtr pApplication);
  ~ModelExplorerWidget();

public:
  void readModelAndShow();

signals:
  void rebuildModelTree();

private slots:
  void setPropertyViewMode(int buttonId);
    
  void buildTreeViewModel();

  void onTreeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
  
  void onTreeViewItemClicked(const QModelIndex& index);

  QModelIndex getSelectedTreeViewItemIndex() const;

  void showSelectedModelObject();
  void unhideSelectedModelObject();
  void hideSelectedModelObject();

  void onRengaObjectSelected(const int modelObjectId);

private:
  QPushButton* createPushButton(const QString& iconPath, const QString& tooltip);
  QToolBar* createToolBar(QWidget* parentWidget);
  
  void createTreeView();
  void createPropertyView();
  void updatePlacement();
  void updateOwner();

  void clearPropertyView();

  void onModelObjectSelected(const QModelIndex& index);
  void onMaterialLayerSelected(const QModelIndex& index);
  void onRebarUsageSelected(const QModelIndex& index);
  void onReinforcementUnitUsageSelected(const QModelIndex& index);
  void onStyleSelected(const QModelIndex& index);

  Renga::IModelObjectPtr getModelObject(int id);
  Renga::IMaterialLayerPtr getMaterialLayer(Renga::IModelObjectPtr pModelObject, int layerIndex);
  Renga::ILayerPtr getLayer(Renga::IModelObjectPtr pModelObject, int layerIndex);
  Renga::ILayeredMaterialPtr getLayeredMaterial(int id);

  Renga::IReinforcementUnitUsagePtr getReinforcementUnitUsage(
    Renga::IModelObjectPtr pModelObject,
    int reinforcementUnitUsageIndex);

  Renga::IRebarUsagePtr getRebarUsage(Renga::IModelObjectPtr pModelObject, int rebarUsageIndex);
  Renga::IRebarUsagePtr getRebarUsage(int reinforcementUnitStyleId, int rebarUsageIndex);

  void selectModelObject(int modelObjectId);

  void updateTreeViewItemVisibility(const QModelIndex& itemIndex, const QModelIndex& visibilityIconIndex);
  void updateTreeViewParentVisibility(const QModelIndex& itemIndex);

  bool isTreeViewItemVisible(const QModelIndex& itemIndex) const;

  void setTreeViewVisibilityIconState(const QModelIndex& visibilityIconIndex, bool isVisible);
  bool getTreeViewVisibilityIconState(const QModelIndex& visibilityIconIndex);

  void setTreeViewItemVisible(const QModelIndex& itemIndex, bool isVisible);

  bool isTreeViewObjectGroup(const QModelIndex& itemIndex) const;
  bool isTreeViewModelObject(const QModelIndex& itemIndex) const;
  
  int getTreeViewModelObjectId(const QModelIndex& itemIndex) const;

  bool isTreeViewObjectGroupVisible(const QModelIndex& itemIndex) const;
  bool isTreeViewModelObjectVisible(const QModelIndex& itemIndex) const;

  ObjectIdList getTreeViewSubtreeObjectList(const QModelIndex& itemIndex) const;

  QList<QModelIndex> getTreeViewSubtreeIndexList(const QModelIndex& rootItemIndex) const;

private:
  Renga::IApplicationPtr m_pApplication;
  std::unique_ptr<Ui::ModelExplorerDialog> m_pUi;
  PropertyView* m_pPropertyView;

  QTreeView* m_pTreeView;
  std::unique_ptr<QStandardItemModel> m_pTreeViewModel;

  std::unique_ptr<ObjectSelectionHandler> m_pObjectSelectionHandler;

  bool m_wasObjectSelectedInCode;
  bool m_wasObjectSelectedInRenga;
};
