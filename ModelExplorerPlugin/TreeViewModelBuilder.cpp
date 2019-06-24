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
}

const std::list<TreeViewModelBuilder::ObjectTypeData>& TreeViewModelBuilder::getLevelObjectTypeData() const
{
  static std::list<ObjectTypeData> levelObjectTypeData{
    ObjectTypeData(Renga::ObjectTypes::Wall, QApplication::translate("me_modelObjects", "Walls"), ":/icons/Wall"),
    ObjectTypeData(Renga::ObjectTypes::Column, QApplication::translate("me_modelObjects", "Columns"), ":/icons/Column"),
    ObjectTypeData(Renga::ObjectTypes::Floor, QApplication::translate("me_modelObjects", "Floors"), ":/icons/Floor"),
    ObjectTypeData(Renga::ObjectTypes::Opening, QApplication::translate("me_modelObjects", "Openings"), ":/icons/Opening"),
    ObjectTypeData(Renga::ObjectTypes::Roof, QApplication::translate("me_modelObjects", "Roofs"), ":/icons/Roof"),
    ObjectTypeData(Renga::ObjectTypes::Beam, QApplication::translate("me_modelObjects", "Beams"), ":/icons/Beam"),
    ObjectTypeData(Renga::ObjectTypes::Stair, QApplication::translate("me_modelObjects", "Stairs"), ":/icons/Stair"),
    ObjectTypeData(Renga::ObjectTypes::Ramp, QApplication::translate("me_modelObjects", "Ramps"), ":/icons/Ramp"),
    ObjectTypeData(Renga::ObjectTypes::Window, QApplication::translate("me_modelObjects", "Windows"), ":/icons/Window"),
    ObjectTypeData(Renga::ObjectTypes::Door, QApplication::translate("me_modelObjects", "Doors"), ":/icons/Door"),
    ObjectTypeData(Renga::ObjectTypes::Railing, QApplication::translate("me_modelObjects", "Railings"), ":/icons/Railing"),
    ObjectTypeData(Renga::ObjectTypes::Room, QApplication::translate("me_modelObjects", "Rooms"), ":/icons/Room"),
    ObjectTypeData(Renga::ObjectTypes::IsolatedFoundation, QApplication::translate("me_modelObjects", "Isolated foundations"), ":/icons/Isolated_foundation"),
    ObjectTypeData(Renga::ObjectTypes::WallFoundation, QApplication::translate("me_modelObjects", "Wall foundations"), ":/icons/Wall_foundation"),
    ObjectTypeData(Renga::ObjectTypes::AssemblyInstance, QApplication::translate("me_modelObjects", "Assembly instances"), ":/icons/Assembly"),
    ObjectTypeData(Renga::ObjectTypes::Element, QApplication::translate("me_modelObjects", "Elements"), ":/icons/Element"),
    ObjectTypeData(Renga::ObjectTypes::Equipment, QApplication::translate("me_modelObjects", "Equipment"), ":/icons/Equipment"),
    ObjectTypeData(Renga::ObjectTypes::PlumbingFixture, QApplication::translate("me_modelObjects", "PlumbingFixtures"), ":/icons/PlumbingFixture"),
    ObjectTypeData(Renga::ObjectTypes::RoutePoint, QApplication::translate("me_modelObjects", "RoutePoints"), ":/icons/RoutePoint"),
    ObjectTypeData(Renga::ObjectTypes::Plate, QApplication::translate("me_modelObjects", "Plates"), ":/icons/Plate")
  };
  return levelObjectTypeData;
}

const std::list<TreeViewModelBuilder::ObjectTypeData>& TreeViewModelBuilder::getNonLevelObjectTypeData() const
{
  static std::list<ObjectTypeData> nonlevelObjectTypeData{
    ObjectTypeData(Renga::ObjectTypes::PipelineAccessory, QApplication::translate("me_modelObjects", "PipeAccessories"), ":/icons/PipeAccessory"),
    ObjectTypeData(Renga::ObjectTypes::PipeFitting, QApplication::translate("me_modelObjects", "PipeFittings"), ":/icons/PipeFitting"),
    ObjectTypeData(Renga::ObjectTypes::Pipe, QApplication::translate("me_modelObjects", "Pipes"), ":/icons/Pipe"),
    ObjectTypeData(Renga::ObjectTypes::AirDuct, QApplication::translate("me_modelObjects", "AirDucts"), ":/icons/AirDuct"),
    ObjectTypeData(Renga::ObjectTypes::Route, QApplication::translate("me_modelObjects", "Routes"), ":/icons/Route"),
    ObjectTypeData(Renga::ObjectTypes::Axis, QApplication::translate("me_modelObjects", "Axes"), ":/icons/Axis"),
    ObjectTypeData(Renga::ObjectTypes::Elevation, QApplication::translate("me_modelObjects", "Elevations"), ":/icons/Elevation"),
    ObjectTypeData(Renga::ObjectTypes::Section, QApplication::translate("me_modelObjects", "Sections"), ":/icons/Section")
  };
  return nonlevelObjectTypeData;
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

  auto levels = getLevels(*pModelObjectCollection);
  levels.sort(compareLevelElevations);
  
  for (auto pLevelModelObject : levels)
    addLevelSubtree(pItemModel, pLevelModelObject, pModelObjectCollection);

  addNonLevelSubtree(pItemModel, pModelObjectCollection);

  return pItemModel;
}

std::list<Renga::IModelObjectPtr> TreeViewModelBuilder::getObjectGroup(
  Renga::IModelObjectCollection& objectCollection,
  std::function<bool(Renga::IModelObject&)> groupFilter) const
{
  std::list<Renga::IModelObjectPtr> result;

  for (int i = 0; i < objectCollection.Count; i++)
  {
    Renga::IModelObjectPtr pModelObject = objectCollection.GetByIndex(i);

    if (groupFilter(*pModelObject) == false)
      continue;

    result.push_back(pModelObject);
  }

  return result;
}

std::list<Renga::IModelObjectPtr> TreeViewModelBuilder::getNonLevelObjectsWithType(
  Renga::IModelObjectCollection & objectCollection,
  GUID objectType) const
{
  return getObjectGroup(objectCollection, [=](Renga::IModelObject& object)
  {
    auto currentObjectType = object.ObjectType;
    if (currentObjectType != objectType)
      return false;

    Renga::ILevelObjectPtr pLevelObject;
    object.QueryInterface(&pLevelObject);

    return pLevelObject == nullptr;
  });
}

std::list<Renga::IModelObjectPtr> TreeViewModelBuilder::getLevels(Renga::IModelObjectCollection & objectCollection) const
{
  return getObjectGroup(objectCollection, [](Renga::IModelObject& object)
  {
    return object.GetObjectType() == Renga::ObjectTypes::Level;
  });
}

std::list<Renga::IModelObjectPtr> TreeViewModelBuilder::getLevelObjectsWithType(
  Renga::IModelObjectCollection & objectCollection, 
  int levelId, 
  GUID objectType) const
{
  return getObjectGroup(objectCollection, [=](Renga::IModelObject& object)
  {
    if (object.ObjectType != objectType)
      return false;

    Renga::ILevelObjectPtr pLevelObject;
    object.QueryInterface(&pLevelObject);

    return pLevelObject != nullptr && pLevelObject->LevelId == levelId;
  });
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

void TreeViewModelBuilder::addNonLevelSubtree(QStandardItemModel * pItemModel, Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  auto pItem = createOtherGroupItem();

  for (const auto& objectTypeData : getNonLevelObjectTypeData())
  {
    auto objectGroup = getNonLevelObjectsWithType(pModelObjectCollection, objectTypeData.m_type);
    addObjectGroupSubtree(pItem.at(0), objectGroup, objectTypeData);
  }

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addLevelSubtree(
  QStandardItemModel* pItemModel,
  Renga::IModelObjectPtr pLevelModelObject,
  Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  assert(pLevelModelObject->GetObjectType() == Renga::ObjectTypes::Level);

  auto pItem = createLevelItem(pLevelModelObject);

  for (const auto& objectTypeData : getLevelObjectTypeData())
  {
    auto objectGroup = getLevelObjectsWithType(pModelObjectCollection, pLevelModelObject->Id, objectTypeData.m_type);
    addObjectGroupSubtree(pItem.at(0), objectGroup, objectTypeData);
  }

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addObjectGroupSubtree(
  QStandardItem* pParentItem,
  const std::list<Renga::IModelObjectPtr>& objectGroup,
  const ObjectTypeData& objectTypeData)
{
  QList<QStandardItem*> objectGroupItemList =
    createItem(objectTypeData.m_typeNodeName, ":/icons/Folder", ItemType_ObjectGroup, true, true);

  if (objectGroup.empty())
    return;

  for (auto pObject : objectGroup)
  {
    QList<QStandardItem*> itemList = createModelObjectItem(pObject, objectTypeData);
    addMaterialsSubtree(itemList.at(0), pObject);
    objectGroupItemList.at(0)->appendRow(itemList);
  }
  
  setItemVisibilityState(objectGroupItemList, objectGroupHasVisibleObject(objectGroup));
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
    return;
  }

  Renga::IObjectWithMaterialPtr pObjectWithMaterial;
  pModelObject->QueryInterface(&pObjectWithMaterial);
  
  if(pObjectWithMaterial != nullptr && pObjectWithMaterial->HasMaterial())
  {
    addSingleMaterialMaterialSubtree(pParentItem, pModelObject);
    return;
  }
}

void TreeViewModelBuilder::addSingleMaterialMaterialSubtree(
  QStandardItem* pParentItem,
  Renga::IModelObjectPtr pModelObject)
{
  QList<QStandardItem*> materialItemList = createSolidMaterialItem(pModelObject);
  addReinforcementSubtree(materialItemList.first(), pModelObject);
  pParentItem->appendRow(materialItemList);
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

QList<QStandardItem*> TreeViewModelBuilder::createOtherGroupItem() const
{
  return createItem(QApplication::translate("me_modelObjects", "OtherObjectGroup"), 
    ":/icons/Folder", 
    ItemType_ObjectGroup, 
    true, 
    true);
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
  auto name = pModelObject->Name;
  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(name), objectTypeData.m_iconPath, ItemType_ModelObject, true, isModelObjectVisible);

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

bool TreeViewModelBuilder::objectGroupHasVisibleObject(const std::list<Renga::IModelObjectPtr>& objectsGroup) const
{
  for (auto pObject : objectsGroup)
    if (getRengaObjectVisibility(m_pApplication, pObject->Id))
      return true;
  return false;
}
