//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "TreeViewModelBuilder.h"
#include "RengaObjectVisibility.h"

#include <Renga/ObjectTypes.h>
#include <comdef.h>

TreeViewModelBuilder::ObjectTypeData::ObjectTypeData(GUID type, QString translationLiteral, QString iconPath) :
  m_type(type),
  m_typeNodeName(translationLiteral),
  m_iconPath(iconPath)
{
}

TreeViewModelBuilder::TreeViewModelBuilder(Renga::IApplicationPtr pApplication) :
  m_pApplication(pApplication)
{
  addObjectType(Renga::ObjectTypes::Wall, QApplication::translate("me_modelObjects", "Walls"), ":/icons/Wall");
  addObjectType(Renga::ObjectTypes::Column, QApplication::translate("me_modelObjects", "Columns"), ":/icons/Column");
  addObjectType(Renga::ObjectTypes::Floor, QApplication::translate("me_modelObjects", "Floors"), ":/icons/Floor");
  addObjectType(Renga::ObjectTypes::Opening, QApplication::translate("me_modelObjects", "Openings"), ":/icons/Opening");
  addObjectType(Renga::ObjectTypes::Roof, QApplication::translate("me_modelObjects", "Roofs"), ":/icons/Roof");
  addObjectType(Renga::ObjectTypes::Beam, QApplication::translate("me_modelObjects", "Beams"), ":/icons/Beam");
  addObjectType(Renga::ObjectTypes::Stair, QApplication::translate("me_modelObjects", "Stairs"), ":/icons/Stair");
  addObjectType(Renga::ObjectTypes::Ramp, QApplication::translate("me_modelObjects", "Ramps"), ":/icons/Ramp");
  addObjectType(Renga::ObjectTypes::Window, QApplication::translate("me_modelObjects", "Windows"), ":/icons/Window");
  addObjectType(Renga::ObjectTypes::Door, QApplication::translate("me_modelObjects", "Doors"), ":/icons/Door");
  addObjectType(Renga::ObjectTypes::Railing, QApplication::translate("me_modelObjects", "Railings"), ":/icons/Railing");
  addObjectType(Renga::ObjectTypes::Room, QApplication::translate("me_modelObjects", "Rooms"), ":/icons/Room");
  addObjectType(Renga::ObjectTypes::IsolatedFoundation, QApplication::translate("me_modelObjects", "Isolated foundations"), ":/icons/Isolated_foundation");
  addObjectType(Renga::ObjectTypes::WallFoundation, QApplication::translate("me_modelObjects", "Wall foundations"), ":/icons/Wall_foundation");
  addObjectType(Renga::ObjectTypes::AssemblyInstance, QApplication::translate("me_modelObjects", "Assembly instances"), ":/icons/Assembly");
  addObjectType(Renga::ObjectTypes::Element, QApplication::translate("me_modelObjects", "Elements"), ":/icons/Element");
}

static double getLevelElevation(Renga::IModelObjectPtr pModelObject)
{
  Renga::ILevelPtr pLevel;

  pModelObject->QueryInterface(&pLevel);
  assert(pLevel != nullptr);

  return pLevel->GetElevation();
}

static bool compareLevelElevations(Renga::IModelObjectPtr pLeftModelObject, Renga::IModelObjectPtr pRightModelObject)
{
  return getLevelElevation(pLeftModelObject) < getLevelElevation(pRightModelObject);
}

QStandardItemModel* TreeViewModelBuilder::build()
{
  QStandardItemModel* pItemModel = new QStandardItemModel();

  auto pProject = m_pApplication->GetProject();
  auto pModel = pProject->GetModel();
  auto pModelObjectCollection = pModel->GetObjects();

  std::list<Renga::IModelObjectPtr> levels;

  for (int i = 0; i < pModelObjectCollection->Count; i++)
  {
    auto pModelObject = pModelObjectCollection->GetByIndex(i);

    if (pModelObject->GetObjectType() != Renga::ObjectTypes::Level)
      continue;

    levels.push_back(pModelObject);
  }

  levels.sort(compareLevelElevations);

  for (auto pLevelModelObject : levels)
    addLevelSubtree(pItemModel, pLevelModelObject, pModelObjectCollection);

  return pItemModel;
}

bool TreeViewModelBuilder::tryGetItemType(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_ItemType, result);
}

bool TreeViewModelBuilder::tryGetModelObjectId(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_ModelObjectId, result);
}

bool TreeViewModelBuilder::tryGetLayerIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_LayerIndex, result);
}

bool TreeViewModelBuilder::tryGetRebarUsageIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_RebarUsageIndex, result);
}

bool TreeViewModelBuilder::tryGetReinforcementUnitUsageIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_ReinforcementUnitUsageIndex, result);
}

bool TreeViewModelBuilder::tryGetReinforcementUnitStyleId(QStandardItemModel* pItemModel, const QModelIndex& index, int& result)
{
  return tryGetIntegerData(pItemModel, index, Role_ReinforcementUnitStyleId, result);
}

bool TreeViewModelBuilder::tryGetIntegerData(QStandardItemModel* pItemModel, const QModelIndex& index, int role, int& result)
{
  QVariant data = pItemModel->data(index, role);
  if (!data.isValid())
    return false;

  bool ok = false;
  int value = data.toInt(&ok);
  
  if (ok)
    result = value;

  return ok;
}

void TreeViewModelBuilder::addObjectType(GUID type, const QString& translationLiteral, QString iconPath)
{
  m_objectTypeDataArray.push_back(ObjectTypeData(type, translationLiteral, iconPath));
}

void TreeViewModelBuilder::addLevelSubtree(
  QStandardItemModel* pItemModel,
  Renga::IModelObjectPtr pLevelModelObject,
  Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  assert(pLevelModelObject->GetObjectType() == Renga::ObjectTypes::Level);

  auto pItem = createLevelItem(pLevelModelObject);

  for (const auto& objectTypeData : m_objectTypeDataArray)
    addObjectGroupSubtree(pItem.at(0), pModelObjectCollection, objectTypeData, pLevelModelObject->Id);

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addObjectGroupSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectCollectionPtr pModelObjectCollection,
  const TreeViewModelBuilder::ObjectTypeData& objectTypeData,
  int levelId)
{
  QList<QStandardItem*> objectGroupItemList =
    createItem(objectTypeData.m_typeNodeName, ":/icons/Folder", ItemType_ObjectGroup, true, true);

  bool isObjectGroupVisible = false;

  int itemCount = 0;

  for (int i = 0; i < pModelObjectCollection->Count; i++)
  {
    Renga::IModelObjectPtr pModelObject = pModelObjectCollection->GetByIndex(i);

    if (pModelObject->GetObjectType() != objectTypeData.m_type)
      continue;

    Renga::ILevelObjectPtr pLevelObject;
    pModelObject->QueryInterface(&pLevelObject);

    if (pLevelObject == nullptr || pLevelObject->LevelId != levelId)
      continue;

    QList<QStandardItem*> itemList = createModelObjectItem(pModelObject, objectTypeData);
    
    addMaterialsSubtree(itemList.at(0), pModelObject);

    objectGroupItemList.at(0)->appendRow(itemList);

    if (getRengaObjectVisibility(m_pApplication, pModelObject->Id))
      isObjectGroupVisible = true;

    itemCount++;
  }

  setItemVisibilityState(objectGroupItemList, isObjectGroupVisible);

  if (itemCount > 0)
    pParentItem->appendRow(objectGroupItemList);
}

void TreeViewModelBuilder::addMaterialsSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject)
{
  Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
  pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

  if (pObjectWithLayeredMaterial != nullptr && pObjectWithLayeredMaterial->HasLayeredMaterial())
  {
    addLayersSubtree(pParentItem, pModelObject, pObjectWithLayeredMaterial->LayeredMaterialId);
  }
  else
  {
    QList<QStandardItem*> materialItemList = createSolidMaterialItem(pModelObject);

    addReinforcementSubtree(materialItemList.first(), pModelObject);

    pParentItem->appendRow(materialItemList);
  }
}

void TreeViewModelBuilder::addLayersSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject,
  int layeredMaterialId)
{
  Renga::ILayeredMaterialPtr pLayeredMaterial = getLayeredMaterial(layeredMaterialId);
  if (pLayeredMaterial == nullptr)
    return;

  auto pMaterialLayerCollection = pLayeredMaterial->Layers;
  if (pMaterialLayerCollection == nullptr)
    return;

  int baseLayerIndex = pLayeredMaterial->BaseLayerIndex;
  
  int layerCount = pMaterialLayerCollection->Count;

  for (int i = 0; i < layerCount; i++)
  {
    auto pMaterialLayer = pMaterialLayerCollection->Get(i);
    
    QList<QStandardItem*> layerItemList = createMaterialLayerItem(pModelObject, pMaterialLayer, i);

    if (i == baseLayerIndex)
      addReinforcementSubtree(layerItemList.at(0), pModelObject);

    pParentItem->appendRow(layerItemList);
  }
}

void TreeViewModelBuilder::addReinforcementSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject)
{
  Renga::IObjectReinforcementModelPtr pObjectReinforcementModel;

  pModelObject->QueryInterface(&pObjectReinforcementModel);

  if (pObjectReinforcementModel == nullptr)
    return;

  auto pRebarUsageCollection = pObjectReinforcementModel->GetRebarUsages();

  for (int i = 0; i < pRebarUsageCollection->Count; i++)
  {
    auto pRebarUsage = pRebarUsageCollection->Get(i);
    addRebarUsageSubtree(pParentItem, pModelObject, pRebarUsage, i, 0);
  }

  auto pReinforcementUnitUsageCollection = pObjectReinforcementModel->GetReinforcementUnitUsages();

  for (int i = 0; i < pReinforcementUnitUsageCollection->Count; i++)
  {
    auto pReinforcementUnitUsage = pReinforcementUnitUsageCollection->Get(i);
    addReinforcementUnitUsageSubtree(pParentItem, pModelObject, pReinforcementUnitUsage, i);
  }
}

void TreeViewModelBuilder::addRebarUsageSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject,
  Renga::IRebarUsagePtr pRebarUsage,
  int rebarUsageIndex,
  int reinforcementUnitStyleId)
{
  auto pRebarStyle = getRebarStyle(pRebarUsage->StyleId);
  if (pRebarStyle == nullptr)
    return;

  QList<QStandardItem*> rebarUsageItemList =
    createRebarUsageItem(pModelObject, pRebarUsage, rebarUsageIndex, reinforcementUnitStyleId);

  pParentItem->appendRow(rebarUsageItemList);
}

void TreeViewModelBuilder::addReinforcementUnitUsageSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject,
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage,
  int reinforcementUnitUsageIndex)
{
  QList<QStandardItem*> reinforcementUnitUsageItemList =
    createReinforcementUnitUsageItem(pModelObject, pReinforcementUnitUsage, reinforcementUnitUsageIndex);

  auto pReinforcementUnitStyle = getReinforcementUnitStyle(pReinforcementUnitUsage->StyleId);
  
  auto pRebarUsageCollection = pReinforcementUnitStyle->GetRebarUsages();

  for (int i = 0; i < pRebarUsageCollection->Count; i++)
  {
    auto pRebarUsage = pRebarUsageCollection->Get(i);
    addRebarUsageSubtree(reinforcementUnitUsageItemList.first(), pModelObject, pRebarUsage, i, pReinforcementUnitUsage->StyleId);
  }

  pParentItem->appendRow(reinforcementUnitUsageItemList);
}

Renga::IMaterialPtr TreeViewModelBuilder::getMaterial(int materialId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pMaterialManager = pProject->MaterialManager;
  if (pMaterialManager == nullptr)
    return nullptr;

  return pMaterialManager->GetMaterial(materialId);
}

Renga::ILayeredMaterialPtr TreeViewModelBuilder::getLayeredMaterial(int layeredMaterialId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pLayeredMaterialManager = pProject->LayeredMaterialManager;
  if (pLayeredMaterialManager == nullptr)
    return nullptr;

  return pLayeredMaterialManager->GetLayeredMaterial(layeredMaterialId);
}

Renga::IRebarStylePtr TreeViewModelBuilder::getRebarStyle(int rebarStyleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pReinforcementUnitStyleManager = pProject->ReinforcementUnitStyleManager;
  if (pReinforcementUnitStyleManager == nullptr)
    return nullptr;

  return pReinforcementUnitStyleManager->GetRebarStyle(rebarStyleId);
}

Renga::IReinforcementUnitStylePtr TreeViewModelBuilder::getReinforcementUnitStyle(int reinforcementStyleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pReinforcementUnitStyleManager = pProject->ReinforcementUnitStyleManager;
  if (pReinforcementUnitStyleManager == nullptr)
    return nullptr;

  return pReinforcementUnitStyleManager->GetUnitStyle(reinforcementStyleId);
}

QList<QStandardItem*> TreeViewModelBuilder::createLevelItem(Renga::IModelObjectPtr pLevelModelObject) const
{
  bool isLevelVisible = getRengaObjectVisibility(m_pApplication, pLevelModelObject->Id);

  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(pLevelModelObject->Name), ":/icons/Level", ItemType_Level, true, isLevelVisible);

  itemList.first()->setData(pLevelModelObject->Id, Role_ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createModelObjectItem(
  Renga::IModelObjectPtr pModelObject,
  const TreeViewModelBuilder::ObjectTypeData& objectTypeData) const
{
  bool isModelObjectVisible = getRengaObjectVisibility(m_pApplication, pModelObject->Id);

  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(pModelObject->Name), objectTypeData.m_iconPath, ItemType_ModelObject, true, isModelObjectVisible);

  itemList.first()->setData(pModelObject->Id, Role_ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createMaterialLayerItem(
  Renga::IModelObjectPtr pModelObject,
  Renga::IMaterialLayerPtr pMaterialLayer,
  int layerIndex) const
{
  auto pMaterial = pMaterialLayer->GetMaterial();
  
  QString materialName = pMaterial != nullptr ?
    QString::fromWCharArray(pMaterial->Name) :
    QApplication::translate("me_materials", "No material");

  QList<QStandardItem*> itemList = createItem(materialName, ":/icons/Material", ItemType_MaterialLayer);

  itemList.first()->setData(pModelObject->Id, Role_ModelObjectId);
  itemList.first()->setData(layerIndex, Role_LayerIndex);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createSolidMaterialItem(Renga::IModelObjectPtr pModelObject) const
{
  Renga::IObjectWithMaterialPtr pObjectWithMaterial;
  pModelObject->QueryInterface(&pObjectWithMaterial);

  bool hasMaterial = pObjectWithMaterial != nullptr && pObjectWithMaterial->HasMaterial();

  Renga::IMaterialPtr pMaterial = hasMaterial ? getMaterial(pObjectWithMaterial->MaterialId) : nullptr;

  QString materialItemName = pMaterial != nullptr ?
    QString::fromWCharArray(pMaterial->Name) :
    QApplication::translate("me_materials", "No material");

  QList<QStandardItem*> itemList = createItem(materialItemName, ":/icons/Material", ItemType_Undefined);

  itemList.first()->setData(pModelObject->Id, Role_ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createRebarUsageItem(
  Renga::IModelObjectPtr pModelObject,
  Renga::IRebarUsagePtr pRebarUsage,
  int rebarUsageIndex,
  int reinforcementUnitStyleId)
{
  Renga::IRebarStylePtr pRebarStyle = getRebarStyle(pRebarUsage->StyleId);
  assert(pRebarStyle != nullptr);

  QList<QStandardItem*> itemList = createItem(QString::fromWCharArray(pRebarStyle->Name), ":/icons/Rebar", ItemType_RebarUsage);

  itemList.first()->setData(pModelObject->Id, Role_ModelObjectId);

  itemList.first()->setData(rebarUsageIndex, Role_RebarUsageIndex);
  itemList.first()->setData(reinforcementUnitStyleId, Role_ReinforcementUnitStyleId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createReinforcementUnitUsageItem(
  Renga::IModelObjectPtr pModelObject,
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage,
  int reinforcementUnitUsageIndex)
{
  auto pReinforcementUnitStyle = getReinforcementUnitStyle(pReinforcementUnitUsage->StyleId);
  assert(pReinforcementUnitStyle != nullptr);

  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(pReinforcementUnitStyle->Name),
    ":/icons/Reinforcement", ItemType_ReinforcementUnitUsage);

  itemList.first()->setData(pModelObject->Id, Role_ModelObjectId);
  itemList.first()->setData(reinforcementUnitUsageIndex, Role_ReinforcementUnitUsageIndex);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createItem(
  const QString& name,
  const QString& iconPath,
  int itemType,
  bool createVisibilityItem,
  bool isVisible) const
{
  QList<QStandardItem*> itemList;

  auto pItem = new QStandardItem(name);

  pItem->setIcon(QIcon(iconPath));
  pItem->setData(itemType, Role_ItemType);

  itemList.append(pItem);

  if (createVisibilityItem)
  {
    auto pVisibilityItem = new QStandardItem();
    itemList.append(pVisibilityItem);

    setItemVisibilityState(itemList, isVisible);
  }

  return itemList;
}

void TreeViewModelBuilder::setItemVisibilityState(QList<QStandardItem*>& itemList, bool isVisible) const
{
  if (itemList.count() <= 1)
    return;

  QStandardItem* pVisibilityItem = itemList.at(1);

  QString iconPath = isVisible ? ":/icons/Visible" : ":/icons/Hidden";

  pVisibilityItem->setIcon(QIcon(iconPath));
  pVisibilityItem->setData(isVisible, Role_IsVisible);
}
