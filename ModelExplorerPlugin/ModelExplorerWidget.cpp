//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"

#include "ScopeGuard.h"
#include "EntityPropertyViewBuilder.h"
#include "MaterialLayerPropertyViewBuilder.h"
#include "ModelExplorerWidget.h"
#include "RebarUsagePropertyViewBuilder.h"
#include "ReinforcementUnitUsagePropertyViewBuilder.h"
#include "RengaModelUtils.h"
#include "TreeViewItemRole.h"
#include "TreeViewItemType.h"
#include "TreeViewModelBuilder.h"
#include "TreeViewUtils.h"
#include "ui_ModelExplorer.h"

#include <Renga/ObjectTypes.h>

#include <QtCore/QFile.h>
#include <QtWidgets/QButtonGroup.h>
#include <QtCore/QSettings.h>

#include <windows.h>


static const unsigned int c_displacementFromParentTop = 100;
static const unsigned int c_displacementFromParentLeft = 5;

namespace
{
  std::unique_ptr<QSettings> createSettings()
  {
    auto pSettings = std::make_unique<QSettings>("Renga Software", "ModelExplorerPlugin");
    return pSettings;
  }
}

ModelExplorerWidget::ModelExplorerWidget(Renga::IApplicationPtr pApplication) :
  QWidget(nullptr, Qt::Tool),
  m_pApplication(pApplication),
  m_wasObjectSelectedInCode(false),
  m_wasObjectSelectedInRenga(false)
{
  m_pObjectSelectionHandler = std::make_unique<ObjectSelectionHandler>(m_pApplication->GetSelection());

  connect(m_pObjectSelectionHandler.get(),
    SIGNAL(objectSelected(const int&)),
    SLOT(onRengaObjectSelected(const int&)));

  m_pUi.reset(new Ui::ModelExplorerDialog());
  m_pUi->setupUi(this);

  // load main window stylesheet
  QFile styleFile(":/styles/mainFormStyle.qss");
  if(styleFile.open(QIODevice::ReadOnly))
    setStyleSheet(styleFile.readAll());

  // load UI
  QPushButton* pRefreshButton = createPushButton(":/icons/Refresh.png", QApplication::translate("me_propertyView", "refresh"));
  connect(pRefreshButton, SIGNAL(clicked()), this, SIGNAL(rebuildModelTree()));

  QPushButton* pShowButton = createPushButton(":/icons/FindAndSelect.png", QApplication::translate("me_propertyView", "show"));
  QPushButton* pUnhideButton = createPushButton(":/icons/Visible.png", QApplication::translate("me_propertyView", "visible"));
  QPushButton* pHideButton = createPushButton(":/icons/Hidden.png", QApplication::translate("me_propertyView", "hidden"));

  // top toolbar
  QToolBar* pTopToolBar = createToolBar(m_pUi->layoutWidget);
  pTopToolBar->addWidget(pRefreshButton);
  pTopToolBar->addWidget(pShowButton);
  pTopToolBar->addWidget(pUnhideButton);
  pTopToolBar->addWidget(pHideButton);
  m_pUi->topVerticalLayout->addWidget(pTopToolBar);

  // tree view
  createTreeView();
  m_pUi->topVerticalLayout->addWidget(m_pTreeView);

  connect(this, SIGNAL(rebuildModelTree()), this, SLOT(buildTreeViewModel()));

  connect(pShowButton, SIGNAL(clicked()), this, SLOT(showSelectedModelObject()));
  connect(pUnhideButton, SIGNAL(clicked()), this, SLOT(unhideSelectedModelObject()));
  connect(pHideButton, SIGNAL(clicked()), this, SLOT(hideSelectedModelObject()));
  
  // view mode buttons
  QPushButton* pCategoryButton = createPushButton(":/icons/Category.png", QApplication::translate("me_propertyView", "category"));
  pCategoryButton->setCheckable(true);
  pCategoryButton->setChecked(true);

  QPushButton* pListButton = createPushButton(":/icons/List.png", QApplication::translate("me_propertyView", "list"));
  pListButton->setCheckable(true);

  // bottom toolbar
  QToolBar* pBottomToolBar = createToolBar(m_pUi->layoutWidget2);
  pBottomToolBar->addWidget(pCategoryButton);
  pBottomToolBar->addWidget(pListButton);
  m_pUi->bottomVerticalLayout->addWidget(pBottomToolBar);

  // view mode button group
  QButtonGroup* pButtonGroup = new QButtonGroup(pBottomToolBar);
  pButtonGroup->setExclusive(true);
  pButtonGroup->addButton(pCategoryButton, 0);
  pButtonGroup->addButton(pListButton, 1);
  connect(pButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=](int id){setPropertyViewMode(id);});

  createPropertyView();
  m_pUi->bottomVerticalLayout->addWidget(m_pPropertyView);
}

ModelExplorerWidget::~ModelExplorerWidget()
{}

void ModelExplorerWidget::setPropertyViewMode(int pressedButtonId)
{
  m_pPropertyView->changeMode(PropertyView::Mode(pressedButtonId));
}

void ModelExplorerWidget::buildTreeViewModel()
{
  TreeViewModelBuilder treeViewModelBuilder(m_pApplication);
  m_pTreeViewModel.reset(treeViewModelBuilder.build());
  m_pTreeView->setModel(m_pTreeViewModel.get());

  QItemSelectionModel* pItemSelectionModel = m_pTreeView->selectionModel();

  connect(
    pItemSelectionModel,
    SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
    this,
    SLOT(onTreeViewSelectionChanged(const QItemSelection&, const QItemSelection&)));

  const unsigned int visibilityIconColumnSize = 30;

  QHeaderView* treeViewHeader = m_pTreeView->header();
  treeViewHeader->setStretchLastSection(false);
  treeViewHeader->setSectionResizeMode(treeViewHeader->logicalIndex(0), QHeaderView::ResizeMode::Stretch);
  treeViewHeader->setSectionResizeMode(treeViewHeader->logicalIndex(1), QHeaderView::ResizeMode::Fixed);
  treeViewHeader->resizeSection(1, visibilityIconColumnSize);

  pItemSelectionModel->select(m_pTreeViewModel->index(0, 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

void ModelExplorerWidget::onTreeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
  QModelIndexList indexList = selected.indexes();

  if (indexList.empty())
  {
    clearPropertyView();
    return;
  }

  QModelIndex selectedItemIndex = indexList.at(0);

  int itemType = eTreeViewItemType::Undefined;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), selectedItemIndex, eTreeViewItemRole::ItemType, itemType))
    assert(false);

  if (isTreeViewModelObject(selectedItemIndex))
  {
    int modelObjectId = getTreeViewModelObjectId(selectedItemIndex);
    selectModelObject(modelObjectId);
  }
  else
  {
    selectModelObject(0);
  }

  switch (itemType)
  {
  case eTreeViewItemType::ModelObject:
  case eTreeViewItemType::Level:
    onModelObjectSelected(selectedItemIndex);
    break;

  case eTreeViewItemType::MaterialLayer:
    onMaterialLayerSelected(selectedItemIndex);
    break;

  case eTreeViewItemType::ReinforcementUnitUsage:
    onReinforcementUnitUsageSelected(selectedItemIndex);
    break;

  case eTreeViewItemType::RebarUsage:
    onRebarUsageSelected(selectedItemIndex);
    break;

  case eTreeViewItemType::Style:
    onStyleSelected(selectedItemIndex);
    break;

  default:
    clearPropertyView();
    break;
  }
}

void ModelExplorerWidget::onTreeViewItemClicked(const QModelIndex& index)
{
  if (index.column() == 0)
    return;

  QModelIndex itemIndex = m_pTreeViewModel->index(index.row(), 0, index.parent());
  
  QStandardItem* objectItem = m_pTreeViewModel->itemFromIndex(itemIndex);
  
  bool isVisible = isTreeViewItemVisible(itemIndex);

  isVisible = !isVisible;

  setTreeViewItemVisible(itemIndex, isVisible);
}

QModelIndex ModelExplorerWidget::getSelectedTreeViewItemIndex() const
{
  auto pSelectionModel = m_pTreeView->selectionModel();

  QModelIndexList indexList = pSelectionModel->selectedIndexes();

  if (indexList.empty())
    return QModelIndex();

  return indexList.first();
}

void ModelExplorerWidget::showSelectedModelObject()
{
  auto selectedItemIndex = getSelectedTreeViewItemIndex();
  if (!selectedItemIndex.isValid())
    return;

  if (!isTreeViewObjectGroup(selectedItemIndex) && !isTreeViewModelObject(selectedItemIndex))
    return;

  QList<QModelIndex> indexList = getTreeViewSubtreeIndexList(selectedItemIndex);

  ObjectIdList objectIdList;

  for (const QModelIndex& index : indexList)
  {
    if (isTreeViewModelObject(index))
    {
      int modelObjectId = getTreeViewModelObjectId(index);
      objectIdList.push_back(modelObjectId);
    }
  }

  showRengaObjects(m_pApplication, objectIdList);

  updateTreeViewParentVisibility(selectedItemIndex);

  //TODO: [asv] change visibility icons on expanded tree elements for all objects
}

void ModelExplorerWidget::unhideSelectedModelObject()
{
  auto selectedItemIndex = getSelectedTreeViewItemIndex();

  if (selectedItemIndex.isValid())
    setTreeViewItemVisible(selectedItemIndex, true);
}

void ModelExplorerWidget::hideSelectedModelObject()
{
  auto selectedItemIndex = getSelectedTreeViewItemIndex();

  if (selectedItemIndex.isValid())
    setTreeViewItemVisible(selectedItemIndex, false);
}

void ModelExplorerWidget::onRengaObjectSelected(const int modelObjectId)
{
  BoolGuard guard(m_wasObjectSelectedInRenga, true);
  if (!m_wasObjectSelectedInCode)
  {
    QModelIndexList indexList = m_pTreeViewModel->match(m_pTreeViewModel->index(0, 0),
      eTreeViewItemRole::EntityId,
      modelObjectId,
      1,
      Qt::MatchRecursive);

    if (!indexList.empty())
    {
      QItemSelectionModel* pItemSelectionModel = m_pTreeView->selectionModel();

      pItemSelectionModel->setCurrentIndex(
        indexList.first(),
        QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
      pItemSelectionModel->select(
        indexList.first(),
        QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

      m_pTreeView->expand(indexList.first());
    }
  }
}

QPushButton* ModelExplorerWidget::createPushButton(const QString& iconPath, const QString& tooltip)
{
  QIcon buttonIcon;
  buttonIcon.addFile(iconPath, QSize(16, 16), QIcon::Normal, QIcon::Off);

  QPushButton* pButton = new QPushButton(buttonIcon, QString());
  pButton->setMinimumSize(QSize(24, 24));
  pButton->setMaximumSize(QSize(24, 24));
  pButton->setCursor(QCursor(Qt::ArrowCursor));

  // Note: Focus does not go to all the buttons in QButtonGroup
  // https://bugreports.qt.io/browse/QTBUG-52036
  pButton->setFocusPolicy(Qt::StrongFocus);
  pButton->setToolTip(tooltip);
  return pButton;
}

QToolBar* ModelExplorerWidget::createToolBar(QWidget* parentWidget)
{
  QToolBar* pToolBar = new QToolBar(parentWidget);
  pToolBar->setMovable(false);
  pToolBar->setAllowedAreas(Qt::TopToolBarArea);
  pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  pToolBar->setFloatable(false);
  pToolBar->setFocusPolicy(Qt::NoFocus);
  return pToolBar;
}

void ModelExplorerWidget::createTreeView()
{
  m_pTreeView = new QTreeView(m_pUi->layoutWidget);
  m_pTreeView->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
  m_pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_pTreeView->setHeaderHidden(true);

  connect(m_pTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeViewItemClicked(const QModelIndex&)));
}

void ModelExplorerWidget::createPropertyView()
{
  m_pPropertyView = new PropertyView(this, m_pApplication);
  m_pPropertyView->setResizeMode(QtTreePropertyBrowser::Interactive);
  m_pPropertyView->setHeaderVisible(true);
  m_pPropertyView->setAlternatingRowColors(false);
  m_pPropertyView->setSplitterPosition(150);
}

HWND toWinHandle(int id) {
  uint64_t value = id;
  return reinterpret_cast<HWND>(value);
}

void ModelExplorerWidget::updatePlacement()
{
  auto rengaMainWindowHwnd = toWinHandle(m_pApplication->GetMainWindowHandle());

  RECT parentWindowRect;
  GetWindowRect(rengaMainWindowHwnd, &parentWindowRect);

  move(parentWindowRect.left + c_displacementFromParentLeft, parentWindowRect.top + c_displacementFromParentTop);
}

void ModelExplorerWidget::updateOwner()
{
  auto pluginWindowHwnd = (HWND)this->winId();
  auto rengaMainWindowHwnd = toWinHandle(m_pApplication->GetMainWindowHandle());
  SetWindowLongPtr(pluginWindowHwnd, GWLP_HWNDPARENT, (LONG_PTR)rengaMainWindowHwnd);
}

void ModelExplorerWidget::clearPropertyView()
{
  m_pPropertyView->clear();
}

void ModelExplorerWidget::onModelObjectSelected(const QModelIndex& index)
{
  int modelObjectId = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityId, modelObjectId))
    assert(false);

  auto pModelObject = getModelObject(modelObjectId);
  if (pModelObject == nullptr)
    return;

  auto builder = std::make_unique<EntityPropertyViewBuilder>(
      pModelObject->GetParameters(),
      pModelObject->GetProperties(),
      pModelObject->GetQuantities(),
      false);

  auto propertiesAccess = [this, modelObjectId]() 
  { 
    auto pObject = getModelObject(modelObjectId);
    return pObject != nullptr ? pObject->GetProperties() : nullptr;
  };

  m_pPropertyView->showProperties(std::move(builder),
                                  propertiesAccess);
}

void ModelExplorerWidget::onMaterialLayerSelected(const QModelIndex& index)
{
  int modelObjectId = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityId, modelObjectId))
    assert(false);

  auto pModelObject = getModelObject(modelObjectId);
  assert(pModelObject != nullptr);

  int layerIndex = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::LayerIndex, layerIndex))
    assert(false);
  
  auto pMaterialLayer = getMaterialLayer(pModelObject, layerIndex);
  auto pLayer = getLayer(pModelObject, layerIndex);

  auto builder = std::make_unique<MaterialLayerPropertyViewBuilder>(m_pApplication, pMaterialLayer, pLayer);;

  m_pPropertyView->showProperties(std::move(builder), nullptr);
}

void ModelExplorerWidget::onRebarUsageSelected(const QModelIndex& index)
{
  int modelObjectId = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityId, modelObjectId))
    assert(false);

  auto pModelObject = getModelObject(modelObjectId);
  assert(pModelObject != nullptr);

  int reinforcementUnitStyleId = 0;

  tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::ReinforcementUnitStyleId, reinforcementUnitStyleId);

  int rebarUsageIndex = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::RebarUsageIndex, rebarUsageIndex))
    assert(false);

  Renga::IRebarUsagePtr pRebarUsage = reinforcementUnitStyleId != 0 ?
    getRebarUsage(reinforcementUnitStyleId, rebarUsageIndex) :
    getRebarUsage(pModelObject, rebarUsageIndex);
  assert(pRebarUsage != nullptr);

  auto builder = std::make_unique<RebarUsagePropertyViewBuilder>(m_pApplication, pRebarUsage);

  m_pPropertyView->showProperties(std::move(builder), nullptr);
}

void ModelExplorerWidget::onReinforcementUnitUsageSelected(const QModelIndex& index)
{
  int modelObjectId = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityId, modelObjectId))
    assert(false);

  auto pModelObject = getModelObject(modelObjectId);
  assert(pModelObject != nullptr);

  int reinforcementUnitUsageIndex = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::ReinforcementUnitUsageIndex, reinforcementUnitUsageIndex))
    assert(false);

  auto pReinforcementUnitUsage = getReinforcementUnitUsage(pModelObject, reinforcementUnitUsageIndex);
  assert(pReinforcementUnitUsage != nullptr);

  auto builder = std::make_unique<ReinforcementUnitUsagePropertyViewBuilder>(m_pApplication, pReinforcementUnitUsage);

  m_pPropertyView->showProperties(std::move(builder), nullptr);
}

void ModelExplorerWidget::onStyleSelected(const QModelIndex & index)
{
  int entityId = 0;
  GUID entityType;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityId, entityId) || 
      !tryGetGUID(m_pTreeViewModel.get(), index, eTreeViewItemRole::EntityType, entityType))
    assert(false);

  auto pEntityCollection = getProjectEntityCollection(m_pApplication->Project, entityType);
  assert(pEntityCollection != nullptr);

  auto pEntity = pEntityCollection->GetById(entityId);
  assert(pEntity != nullptr);

  Renga::IPropertyContainerPtr parameters;
  pEntity->QueryInterface(&parameters);

  Renga::IPropertyContainerPtr properties;
  pEntity->QueryInterface(&properties);

  auto builder = std::make_unique<EntityPropertyViewBuilder>(
    parameters,
    properties,
    nullptr,
    true);

  auto propertiesAccess = [entityId, pEntityCollection]() -> Renga::IPropertyContainerPtr
  {
    auto pEntity = pEntityCollection->GetById(entityId);
    if (pEntity == nullptr)
      return nullptr;

    auto properties = Renga::IPropertyContainerPtr();
    pEntity->QueryInterface(&properties);
    return properties;
  };

  m_pPropertyView->showProperties(std::move(builder),
                                  propertiesAccess);
}

Renga::IModelObjectPtr ModelExplorerWidget::getModelObject(int id)
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pModel = pProject->Model;
  if (pModel == nullptr)
    return nullptr;

  auto pModelObjectCollection = pModel->GetObjects();
  if (pModelObjectCollection == nullptr)
    return nullptr;

  return pModelObjectCollection->GetById(id);
}

Renga::IMaterialLayerPtr ModelExplorerWidget::getMaterialLayer(Renga::IModelObjectPtr pModelObject, int layerIndex)
{
  Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
  
  pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

  if (pObjectWithLayeredMaterial == nullptr)
    return nullptr;

  auto pLayeredMaterial = getLayeredMaterial(pObjectWithLayeredMaterial->LayeredMaterialId);

  if (pLayeredMaterial == nullptr)
    return nullptr;

  return pLayeredMaterial->Layers->Get(layerIndex);
}

Renga::ILayerPtr ModelExplorerWidget::getLayer(Renga::IModelObjectPtr pModelObject, int layerIndex)
{
  Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;

  pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

  if (pObjectWithLayeredMaterial == nullptr)
    return nullptr;

  auto pLayerCollection = pObjectWithLayeredMaterial->GetLayers();
  if (pLayerCollection == nullptr)
    return nullptr;

  return pLayerCollection->Get(layerIndex);
}

Renga::ILayeredMaterialPtr ModelExplorerWidget::getLayeredMaterial(int id)
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->LayeredMaterialManager->GetLayeredMaterial(id);
}

Renga::IReinforcementUnitUsagePtr ModelExplorerWidget::getReinforcementUnitUsage(
  Renga::IModelObjectPtr pModelObject,
  int reinforcementUnitUsageIndex)
{
  Renga::IObjectReinforcementModelPtr pObjectReinforcementModel;

  pModelObject->QueryInterface(&pObjectReinforcementModel);

  if (pObjectReinforcementModel == nullptr)
    return nullptr;

  auto pReinforcementUnitUsageCollection = pObjectReinforcementModel->GetReinforcementUnitUsages();

  if (reinforcementUnitUsageIndex < 0 || reinforcementUnitUsageIndex >= pReinforcementUnitUsageCollection->Count)
    return nullptr;

  return pReinforcementUnitUsageCollection->Get(reinforcementUnitUsageIndex);
}

Renga::IRebarUsagePtr ModelExplorerWidget::getRebarUsage(Renga::IModelObjectPtr pModelObject, int rebarUsageIndex)
{
  Renga::IObjectReinforcementModelPtr pObjectReinforcementModel;

  pModelObject->QueryInterface(&pObjectReinforcementModel);

  if (pObjectReinforcementModel == nullptr)
    return nullptr;

  auto pRebarUsageCollection = pObjectReinforcementModel->GetRebarUsages();

  if (rebarUsageIndex < 0 || rebarUsageIndex >= pRebarUsageCollection->Count)
    return nullptr;

  return pRebarUsageCollection->Get(rebarUsageIndex);
}

Renga::IRebarUsagePtr ModelExplorerWidget::getRebarUsage(int reinforcementUnitStyleId, int rebarUsageIndex)
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pReinforcementUnitStyle = pProject->ReinforcementUnitStyleManager->GetUnitStyle(reinforcementUnitStyleId);
  if (pReinforcementUnitStyle == nullptr)
    return nullptr;

  auto pRebarUsageCollection = pReinforcementUnitStyle->GetRebarUsages();

  if (rebarUsageIndex < 0 || rebarUsageIndex >= pRebarUsageCollection->Count)
    return nullptr;

  return pRebarUsageCollection->Get(rebarUsageIndex);
}

void ModelExplorerWidget::selectModelObject(int modelObjectId)
{
  if (m_wasObjectSelectedInRenga)
    return;
  BoolGuard guard(m_wasObjectSelectedInCode, true);

  CComSafeArray<int> objectIds(static_cast<ULONG>(1));
  objectIds.SetAt(0, modelObjectId);

  auto pSelection = m_pApplication->Selection;
  pSelection->SetSelectedObjects(objectIds);
}

void ModelExplorerWidget::updateTreeViewItemVisibility(const QModelIndex& itemIndex, const QModelIndex& visibilityIconIndex)
{
  QStandardItem* item = m_pTreeViewModel->itemFromIndex(itemIndex);

  int itemType = eTreeViewItemType::Undefined;

  bool isVisible = true;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), itemIndex, eTreeViewItemRole::ItemType, itemType))
    assert(false);

  if (itemType == eTreeViewItemType::ModelObject)
  {
    int modelObjectId = 0;

    if (!tryGetIntegerData(m_pTreeViewModel.get(), itemIndex, eTreeViewItemRole::EntityId, modelObjectId))
      assert(false);

    isVisible = getRengaObjectVisibility(m_pApplication, modelObjectId);
  }
  else if (itemType == eTreeViewItemType::ObjectGroup)
  {
    isVisible = isTreeViewObjectGroupVisible(itemIndex);
  }

  setTreeViewVisibilityIconState(visibilityIconIndex, isVisible);
}

void ModelExplorerWidget::updateTreeViewParentVisibility(const QModelIndex& itemIndex)
{
  QStandardItem* pItem = m_pTreeViewModel->itemFromIndex(itemIndex);

  QStandardItem* pParentItem = pItem->parent();
  if (pParentItem == nullptr)
    return;

  QModelIndex parentIndex = pParentItem->index();

  if (isTreeViewObjectGroup(parentIndex))
  {
    bool isVisible = isTreeViewObjectGroupVisible(parentIndex);

    QModelIndex parentVisibilityIconIndex = m_pTreeViewModel->index(parentIndex.row(), 1, parentIndex.parent());
    setTreeViewVisibilityIconState(parentVisibilityIconIndex, isVisible);
  }

  updateTreeViewParentVisibility(pParentItem->index());
}

bool ModelExplorerWidget::isTreeViewItemVisible(const QModelIndex& itemIndex) const
{
  if (isTreeViewObjectGroup(itemIndex))
    return isTreeViewObjectGroupVisible(itemIndex);

  if (isTreeViewModelObject(itemIndex))
    return isTreeViewModelObjectVisible(itemIndex);

  return true;
}

void ModelExplorerWidget::setTreeViewVisibilityIconState(const QModelIndex& visibilityIconIndex, bool isVisible)
{
  QStandardItem* pItem = m_pTreeViewModel->itemFromIndex(visibilityIconIndex);

  QString iconPath = isVisible ? ":/icons/Visible" : ":/icons/Hidden";

  pItem->setIcon(QIcon(iconPath));
  pItem->setData(isVisible, eTreeViewItemRole::IsVisible);
}

bool ModelExplorerWidget::getTreeViewVisibilityIconState(const QModelIndex& visibilityIconIndex)
{
  QStandardItem* pItem = m_pTreeViewModel->itemFromIndex(visibilityIconIndex);
  assert(pItem != nullptr);

  QVariant data = pItem->data(eTreeViewItemRole::IsVisible);
  assert(data.isValid());

  return data.toBool();
}

void ModelExplorerWidget::setTreeViewItemVisible(const QModelIndex& itemIndex, bool isVisible)
{
  if (!isTreeViewObjectGroup(itemIndex) && !isTreeViewModelObject(itemIndex))
    return;

  QList<QModelIndex> indexList = getTreeViewSubtreeIndexList(itemIndex);

  ObjectIdList objectIdList;

  for (const QModelIndex& index : indexList)
  {
    if (isTreeViewModelObject(index))
    {
      int modelObjectId = getTreeViewModelObjectId(index);
      objectIdList.push_back(modelObjectId);
    }

    if (isTreeViewModelObject(index) || isTreeViewObjectGroup(index))
    {
      QModelIndex visibilityIconIndex = m_pTreeViewModel->index(index.row(), 1, index.parent());
      setTreeViewVisibilityIconState(visibilityIconIndex, isVisible);
    }
  }

  setRengaObjectVisibility(m_pApplication, objectIdList, isVisible);

  updateTreeViewParentVisibility(itemIndex);
}

bool ModelExplorerWidget::isTreeViewObjectGroup(const QModelIndex& itemIndex) const
{
  int itemType = eTreeViewItemType::Undefined;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), itemIndex, eTreeViewItemRole::ItemType, itemType))
    return false;

  return itemType == eTreeViewItemType::ObjectGroup;
}

bool ModelExplorerWidget::isTreeViewModelObject(const QModelIndex& itemIndex) const
{
  int itemType = eTreeViewItemType::Undefined;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), itemIndex, eTreeViewItemRole::ItemType, itemType))
    return false;

  return itemType == eTreeViewItemType::ModelObject || itemType == eTreeViewItemType::Level;
}

int ModelExplorerWidget::getTreeViewModelObjectId(const QModelIndex& itemIndex) const
{
  int result = 0;

  if (!tryGetIntegerData(m_pTreeViewModel.get(), itemIndex,eTreeViewItemRole::EntityId , result))
    assert(false);

  return result;
}

bool ModelExplorerWidget::isTreeViewObjectGroupVisible(const QModelIndex& itemIndex) const
{
  bool isVisible = false;

  QStandardItem* item = m_pTreeViewModel->itemFromIndex(itemIndex);

  for (int i = 0; i < item->rowCount(); ++i)
  {
    auto childIndex = item->child(i)->index();
    
    if (isTreeViewModelObject(childIndex) && isTreeViewModelObjectVisible(childIndex))
    {
      isVisible = true;
      break;
    }
    else if (isTreeViewObjectGroup(childIndex) && isTreeViewObjectGroupVisible(childIndex))
    {
      isVisible = true;
      break;
    }
  }

  return isVisible;
}

bool ModelExplorerWidget::isTreeViewModelObjectVisible(const QModelIndex& itemIndex) const
{
  assert(isTreeViewModelObject(itemIndex));
  
  int modelObjectId = getTreeViewModelObjectId(itemIndex);

  return getRengaObjectVisibility(m_pApplication, modelObjectId);
}

ObjectIdList ModelExplorerWidget::getTreeViewSubtreeObjectList(const QModelIndex& itemIndex) const
{
  ObjectIdList result;

  if (isTreeViewModelObject(itemIndex))
    result.push_back(getTreeViewModelObjectId(itemIndex));

  return result;
}

QList<QModelIndex> ModelExplorerWidget::getTreeViewSubtreeIndexList(const QModelIndex& rootItemIndex) const
{
  QList<QModelIndex> indexList;

  indexList.append(rootItemIndex);

  QStandardItem* pItem = m_pTreeViewModel->itemFromIndex(rootItemIndex);

  for (int i = 0; i < pItem->rowCount(); ++i)
  {
    QModelIndex childItemIndex = pItem->child(i)->index();
    indexList.append(getTreeViewSubtreeIndexList(childItemIndex));
  }

  return indexList;
}

void ModelExplorerWidget::readModelAndShow()
{
  emit rebuildModelTree();

  if (!restoreGeometryFromSettings())
    updatePlacement();

  show();
  activateWindow();
  updateOwner();
}

void ModelExplorerWidget::closeEvent(QCloseEvent* pEvent)
{
  saveGeometryToSettings();

  QWidget::closeEvent(pEvent);
}

void ModelExplorerWidget::saveGeometryToSettings()
{
  auto pSettings = createSettings();
  pSettings->setValue("geometry", saveGeometry());
}

bool ModelExplorerWidget::restoreGeometryFromSettings()
{
  auto pSettings = createSettings();
  return restoreGeometry(pSettings->value("geometry").toByteArray());
}
