//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "TreeViewItemRole.h"
#include "TreeViewItemType.h"
#include "TreeViewModelBuilder.h"
#include "RengaObjectVisibility.h"
#include "RengaEntityUIData.h"

#include <Renga/ObjectTypes.h>
#include <Renga/ParameterIds.h>
#include <Renga/StyleTypeIds.h>

#include <comdef.h>


namespace
{
  const std::list<GUID> c_levelTreeTypes =
  {
    Renga::ObjectTypes::Wall,    Renga::ObjectTypes::Column,    Renga::ObjectTypes::Floor,    Renga::ObjectTypes::Opening,    Renga::ObjectTypes::Roof,    Renga::ObjectTypes::Beam,    Renga::ObjectTypes::Stair,    Renga::ObjectTypes::Ramp,    Renga::ObjectTypes::Window,    Renga::ObjectTypes::Door,    Renga::ObjectTypes::Railing,    Renga::ObjectTypes::Room,    Renga::ObjectTypes::IsolatedFoundation,    Renga::ObjectTypes::WallFoundation,    Renga::ObjectTypes::AssemblyInstance,    Renga::ObjectTypes::Element,    Renga::ObjectTypes::Plate,
    Renga::ObjectTypes::RoutePoint,    Renga::ObjectTypes::Equipment,    Renga::ObjectTypes::PlumbingFixture,    Renga::ObjectTypes::MechanicalEquipment,
    Renga::ObjectTypes::Line3D,    Renga::ObjectTypes::Hatch,    Renga::ObjectTypes::TextShape,
    Renga::ObjectTypes::Rebar  };

  const std::list<GUID> c_nonLevelTreeTypes
  {
    Renga::ObjectTypes::PipeAccessory,
    Renga::ObjectTypes::PipeFitting,
    Renga::ObjectTypes::Pipe,
    Renga::ObjectTypes::Duct,
    Renga::ObjectTypes::DuctAccessory,
    Renga::ObjectTypes::DuctFitting,
    Renga::ObjectTypes::LineElectricalCircuit,
    Renga::ObjectTypes::LightFixture,
    Renga::ObjectTypes::ElectricDistributionBoard,
    Renga::ObjectTypes::WiringAccessory,
    Renga::ObjectTypes::Route,
    Renga::ObjectTypes::Axis,
    Renga::ObjectTypes::Elevation,
    Renga::ObjectTypes::Section,
    Renga::ObjectTypes::Dimension,
  };
}


TreeViewModelBuilder::TreeViewModelBuilder(Renga::IApplicationPtr pApplication) :
  m_pApplication(pApplication)
{
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

  processModelObjectCollection(pModelObjectCollection);

  m_levels.sort(compareLevelElevations);
  
  for (auto pLevelModelObject : m_levels)
    addLevelSubtree(pItemModel, pLevelModelObject, pModelObjectCollection);

  addNonLevelSubtree(pItemModel, pModelObjectCollection);

  return pItemModel;
}

void TreeViewModelBuilder::processModelObjectCollection(Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  m_levels.clear();
  m_levelObjects.clear();
  m_nonLevelObjects.clear();

  for (int i = 0; i < pModelObjectCollection->Count; i++)
  {
    auto pModelObject = pModelObjectCollection->GetByIndex(i);

    if (pModelObject->GetObjectType() == Renga::ObjectTypes::Level)
    {
      m_levels.push_back(pModelObject);
    }
    else
    {
      Renga::ILevelObjectPtr pLevelObject;
      pModelObject->QueryInterface(&pLevelObject);

      if (pLevelObject)
        processLevelObject(pModelObject);
      else
        processNonLevelObject(pModelObject);
    }
  }
}

void TreeViewModelBuilder::processLevelObject(Renga::IModelObjectPtr pModelObject)
{
  Renga::ILevelObjectPtr pLevelObject;
  pModelObject->QueryInterface(&pLevelObject);
  assert(pLevelObject != nullptr);

  LevelObjectGroup group(pLevelObject->GetLevelId(), pModelObject->GetObjectType());

  m_levelObjects[group].push_back(pModelObject);
}

void TreeViewModelBuilder::processNonLevelObject(Renga::IModelObjectPtr pNonLevelObject)
{
  m_nonLevelObjects[pNonLevelObject->GetObjectType()].push_back(pNonLevelObject);
}

void TreeViewModelBuilder::addNonLevelSubtree(QStandardItemModel * pItemModel, Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  auto pItem = createOtherGroupItem();

  for (const auto& objectType : c_nonLevelTreeTypes)
    addObjectGroupSubtree(pItem.at(0), m_nonLevelObjects[objectType], getRengaEntityUIData(objectType).pluralName);

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addLevelSubtree(
  QStandardItemModel* pItemModel,
  Renga::IModelObjectPtr pLevelModelObject,
  Renga::IModelObjectCollectionPtr pModelObjectCollection)
{
  assert(pLevelModelObject->GetObjectType() == Renga::ObjectTypes::Level);

  auto pItem = createLevelItem(pLevelModelObject);

  for (const auto& objectType : c_levelTreeTypes)
  {
    LevelObjectGroup group(pLevelModelObject->GetId(), objectType);
    addObjectGroupSubtree(pItem.at(0), m_levelObjects[group], getRengaEntityUIData(objectType).pluralName);
  }

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addObjectGroupSubtree(
    QStandardItem* pParentItem,
    const std::list<Renga::IModelObjectPtr>& objectGroup,
  const QString groupName)
{
  QList<QStandardItem*> objectGroupItemList =
    createItem(groupName, ":/icons/Folder", eTreeViewItemType::ObjectGroup, true, true);

  if (objectGroup.empty())
    return;

  for (auto pObject : objectGroup)
    addObjectSubtree(objectGroupItemList.at(0), pObject);
  
  setItemVisibilityState(objectGroupItemList, objectGroupHasVisibleObject(objectGroup));
  pParentItem->appendRow(objectGroupItemList);
}

const std::map<GUID, GUID>& TreeViewModelBuilder::parameterIdToEntityTypeDict() const
{
  using namespace Renga;
  //static auto less = [](GUID lhs, GUID rhs) { return true; };
  static const std::map <GUID, GUID> dict = {
      {ParameterIds::MaterialStyleId, StyleTypeIds::Material},
      {ParameterIds::LayeredMaterialStyleId, StyleTypeIds::LayeredMaterial}
  };

  return dict;
}

void TreeViewModelBuilder::addObjectSubtree(QStandardItem* pParentItem, Renga::IModelObjectPtr pObject)
{
  QList<QStandardItem*> itemList = createModelObjectItem(pObject);

  auto pObjectParameters = pObject->GetParameters();
  auto pIds = pObjectParameters->GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    auto id = pIds->Get(i);
    auto pParameter = pObjectParameters->Get(id);
    if (pParameter->Definition->_ParameterType != Renga::ParameterType::ParameterType_IntID)
      continue;

    auto entityTypeIt = parameterIdToEntityTypeDict().find(id);
    if(entityTypeIt != parameterIdToEntityTypeDict().cend())
      addStyleSubtree(itemList.at(0), pObject, entityTypeIt->second, pParameter->GetIntValue());
  }

  pParentItem->appendRow(itemList);
}

void TreeViewModelBuilder::addStyleSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject,
    GUID styleType,
    int id)
{
  if (styleType == Renga::StyleTypeIds::LayeredMaterial)
  {
    Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
    pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

    if (pObjectWithLayeredMaterial != nullptr && pObjectWithLayeredMaterial->HasLayeredMaterial())
      addLayersSubtree(pParentItem, pModelObject, pObjectWithLayeredMaterial->LayeredMaterialId);
  }
  else if (styleType == Renga::StyleTypeIds::Material)
  {
    Renga::IObjectWithMaterialPtr pObjectWithMaterial;
    pModelObject->QueryInterface(&pObjectWithMaterial);

    if (pObjectWithMaterial != nullptr && pObjectWithMaterial->HasMaterial())
      addSingleMaterialMaterialSubtree(pParentItem, pModelObject);
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
    eTreeViewItemType::ObjectGroup, 
    true, 
    true);
}

QList<QStandardItem*> TreeViewModelBuilder::createLevelItem(Renga::IModelObjectPtr pLevelModelObject) const
{
  bool isLevelVisible = getRengaObjectVisibility(m_pApplication, pLevelModelObject->Id);

  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(pLevelModelObject->Name), ":/icons/Level", eTreeViewItemType::Level, true, isLevelVisible);

  itemList.first()->setData(pLevelModelObject->Id, eTreeViewItemRole::ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createModelObjectItem(Renga::IModelObjectPtr pModelObject) const
{
  bool isModelObjectVisible = getRengaObjectVisibility(m_pApplication, pModelObject->Id);
  auto name = QString::fromWCharArray(pModelObject->Name);
  auto iconPath = getRengaEntityUIData(pModelObject->ObjectType).icon16Path;
  QList<QStandardItem*> itemList =
      createItem(name, iconPath, eTreeViewItemType::ModelObject, true, isModelObjectVisible);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);

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

  QList<QStandardItem*> itemList = createItem(materialName, ":/icons/Material", eTreeViewItemType::MaterialLayer);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);
  itemList.first()->setData(layerIndex, eTreeViewItemRole::LayerIndex);

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

  QList<QStandardItem*> itemList = createItem(materialItemName, ":/icons/Material", eTreeViewItemType::Undefined);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);

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

  QList<QStandardItem*> itemList = createItem(QString::fromWCharArray(pRebarStyle->Name), ":/icons/Rebar", eTreeViewItemType::RebarUsage);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);

  itemList.first()->setData(rebarUsageIndex, eTreeViewItemRole::RebarUsageIndex);
  itemList.first()->setData(reinforcementUnitStyleId, eTreeViewItemRole::ReinforcementUnitStyleId);

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
    ":/icons/Reinforcement", eTreeViewItemType::ReinforcementUnitUsage);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);
  itemList.first()->setData(reinforcementUnitUsageIndex, eTreeViewItemRole::ReinforcementUnitUsageIndex);

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
  pItem->setData(itemType, eTreeViewItemRole::ItemType);

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
  pVisibilityItem->setData(isVisible, eTreeViewItemRole::IsVisible);
}

bool TreeViewModelBuilder::objectGroupHasVisibleObject(const std::list<Renga::IModelObjectPtr>& objectsGroup) const
{
  for (auto pObject : objectsGroup)
    if (getRengaObjectVisibility(m_pApplication, pObject->Id))
      return true;
  return false;
}
