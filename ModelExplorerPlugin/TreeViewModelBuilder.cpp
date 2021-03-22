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


using namespace Renga;

namespace
{
  const std::list<GUID> c_levelTreeTypes =
  {
    ObjectTypes::Wall,
    ObjectTypes::Column,
    ObjectTypes::Floor,
    ObjectTypes::Opening,
    ObjectTypes::Roof,
    ObjectTypes::Beam,
    ObjectTypes::Stair,
    ObjectTypes::Ramp,
    ObjectTypes::Window,
    ObjectTypes::Door,
    ObjectTypes::Railing,
    ObjectTypes::Room,
    ObjectTypes::IsolatedFoundation,
    ObjectTypes::WallFoundation,
    ObjectTypes::AssemblyInstance,
    ObjectTypes::Element,
    ObjectTypes::Plate,
    ObjectTypes::RoutePoint,
    ObjectTypes::Equipment,
    ObjectTypes::PlumbingFixture,
    ObjectTypes::MechanicalEquipment,
    ObjectTypes::Line3D,
    ObjectTypes::Hatch,
    ObjectTypes::TextShape,
    ObjectTypes::Rebar
  };

  const std::list<GUID> c_nonLevelTreeTypes
  {
    ObjectTypes::PipeAccessory,
    ObjectTypes::PipeFitting,
    ObjectTypes::Pipe,
    ObjectTypes::Duct,
    ObjectTypes::DuctAccessory,
    ObjectTypes::DuctFitting,
    ObjectTypes::LineElectricalCircuit,
    ObjectTypes::LightFixture,
    ObjectTypes::ElectricDistributionBoard,
    ObjectTypes::WiringAccessory,
    ObjectTypes::Route,
    ObjectTypes::Axis,
    ObjectTypes::Elevation,
    ObjectTypes::Section,
    ObjectTypes::Dimension,
  };

  const std::map <GUID, GUID> c_parameterToIdDict =
  {
    { ParameterIds::MaterialStyleId, StyleTypeIds::Material },
    { ParameterIds::LayeredMaterialStyleId, StyleTypeIds::LayeredMaterial },
    { ParameterIds::PlumbingFixtureStyleId, StyleTypeIds::PlumbingFixtureStyle },
    { ParameterIds::MepEquipmentStyleId, StyleTypeIds::EquipmentStyle },
    { ParameterIds::MepPipeStyleId, StyleTypeIds::PipeStyle },
    { ParameterIds::PipeComponentStyleId, StyleTypeIds::PipeFittingStyle },
    { ParameterIds::PipeAccessoryStyleId, StyleTypeIds::PipeAccessoryStyle },
    { ParameterIds::AirEquipmentStyleId, StyleTypeIds::MechanicalEquipmentStyle },
    { ParameterIds::AirDuctStyleId, StyleTypeIds::DuctStyle },
    { ParameterIds::AirComponentStyleId, StyleTypeIds::DuctFittingStyle },
    { ParameterIds::AirAccessoryStyleId, StyleTypeIds::DuctAccessoryStyle },
    { ParameterIds::WiringAccessoryStyleId, StyleTypeIds::WiringAccessoryStyle },
    { ParameterIds::LightFixtureStyleId, StyleTypeIds::LightFixtureStyle },
    { ParameterIds::DistributionBoardStyleId, StyleTypeIds::ElectricDistributionBoardStyle },
    { ParameterIds::ConductorStyleId, StyleTypeIds::ElectricalConductorStyle },
    { ParameterIds::LineElectricalCircuitStyleId, StyleTypeIds::ElectricCircuitLineStyle },
    { ParameterIds::MepSystemStyleId, StyleTypeIds::SystemStyle },
  };
}

TreeViewModelBuilder::TreeViewModelBuilder(IApplicationPtr pApplication) :
  m_pApplication(pApplication)
{
}

static double getLevelElevation(IModelObjectPtr pModelObject)
{
  ILevelPtr pLevel;

  pModelObject->QueryInterface(&pLevel);
  assert(pLevel != nullptr);

  return pLevel->GetElevation();
}

static bool compareLevelElevations(IModelObjectPtr pLeftModelObject, IModelObjectPtr pRightModelObject)
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

void TreeViewModelBuilder::processModelObjectCollection(IModelObjectCollectionPtr pModelObjectCollection)
{
  m_levels.clear();
  m_levelObjects.clear();
  m_nonLevelObjects.clear();

  for (int i = 0; i < pModelObjectCollection->Count; i++)
  {
    auto pModelObject = pModelObjectCollection->GetByIndex(i);

    if (pModelObject->GetObjectType() == ObjectTypes::Level)
    {
      m_levels.push_back(pModelObject);
    }
    else
    {
      ILevelObjectPtr pLevelObject;
      pModelObject->QueryInterface(&pLevelObject);

      if (pLevelObject)
        processLevelObject(pModelObject);
      else
        processNonLevelObject(pModelObject);
    }
  }
}

void TreeViewModelBuilder::processLevelObject(IModelObjectPtr pModelObject)
{
  ILevelObjectPtr pLevelObject;
  pModelObject->QueryInterface(&pLevelObject);
  assert(pLevelObject != nullptr);

  LevelObjectGroup group(pLevelObject->GetLevelId(), pModelObject->GetObjectType());

  m_levelObjects[group].push_back(pModelObject);
}

void TreeViewModelBuilder::processNonLevelObject(IModelObjectPtr pNonLevelObject)
{
  m_nonLevelObjects[pNonLevelObject->GetObjectType()].push_back(pNonLevelObject);
}

void TreeViewModelBuilder::addNonLevelSubtree(QStandardItemModel * pItemModel, IModelObjectCollectionPtr pModelObjectCollection)
{
  auto pItem = createOtherGroupItem();

  for (const auto& objectType : c_nonLevelTreeTypes)
    addObjectGroupSubtree(pItem.at(0), m_nonLevelObjects[objectType], getRengaEntityUIData(objectType).pluralName);

  pItemModel->appendRow(pItem);
}

void TreeViewModelBuilder::addLevelSubtree(
  QStandardItemModel* pItemModel,
  IModelObjectPtr pLevelModelObject,
  IModelObjectCollectionPtr pModelObjectCollection)
{
  assert(pLevelModelObject->GetObjectType() == ObjectTypes::Level);

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
  const std::list<IModelObjectPtr>& objectGroup,
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

void TreeViewModelBuilder::addObjectSubtree(QStandardItem* pParentItem, IModelObjectPtr pObject)
{
  QList<QStandardItem*> itemList = createModelObjectItem(pObject);

  auto pObjectParameters = pObject->GetParameters();
  auto pIds = pObjectParameters->GetIds();
  for (int i = 0; i < pIds->Count; ++i)
  {
    auto id = pIds->Get(i);
    auto pParameter = pObjectParameters->Get(id);
    if (pParameter->Definition->_ParameterType != ParameterType::ParameterType_IntID)
      continue;

    auto entityTypeIt = c_parameterToIdDict.find(id);
    if (entityTypeIt != c_parameterToIdDict.cend())
      addStyleSubtree(itemList.at(0), pObject, entityTypeIt->second, pParameter->GetIntValue());
  }

  pParentItem->appendRow(itemList);
}

void TreeViewModelBuilder::addStyleSubtree(
  QStandardItem* pParentItem,
  IModelObjectPtr pModelObject,
  GUID styleType,
  int id)
{
  if (styleType == StyleTypeIds::LayeredMaterial)
  {
    IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
    pModelObject->QueryInterface(&pObjectWithLayeredMaterial);

    if (pObjectWithLayeredMaterial != nullptr && pObjectWithLayeredMaterial->HasLayeredMaterial())
      addLayersSubtree(pParentItem, pModelObject, pObjectWithLayeredMaterial->LayeredMaterialId);
  }
  else if (styleType == StyleTypeIds::Material)
  {
    IObjectWithMaterialPtr pObjectWithMaterial;
    pModelObject->QueryInterface(&pObjectWithMaterial);

    if (pObjectWithMaterial != nullptr && pObjectWithMaterial->HasMaterial())
      addSingleMaterialMaterialSubtree(pParentItem, pModelObject);
  }
  // TODO: replace with universal IProject method
  else if (styleType == StyleTypeIds::PlumbingFixtureStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->PlumbingFixtureStyles->GetById(id));
  else if (styleType == StyleTypeIds::EquipmentStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->EquipmentStyles->GetById(id));
  else if (styleType == StyleTypeIds::PipeStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->PipeStyles->GetById(id));
  else if (styleType == StyleTypeIds::PipeAccessoryStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->PipeAccessoryStyles->GetById(id));
  else if (styleType == StyleTypeIds::PipeFittingStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->PipeFittingStyles->GetById(id));
  else if (styleType == StyleTypeIds::MechanicalEquipmentStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->MechanicalEquipmentStyles->GetById(id));
  else if (styleType == StyleTypeIds::DuctStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->DuctStyles->GetById(id));
  else if (styleType == StyleTypeIds::DuctFittingStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->DuctFittingStyles->GetById(id));
  else if (styleType == StyleTypeIds::DuctAccessoryStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->DuctAccessoryStyles->GetById(id));
  else if (styleType == StyleTypeIds::WiringAccessoryStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->WiringAccessoryStyles->GetById(id));
  else if (styleType == StyleTypeIds::LightFixtureStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->LightFixtureStyles->GetById(id));
  else if (styleType == StyleTypeIds::ElectricDistributionBoardStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->ElectricDistributionBoardStyles->GetById(id));
  else if (styleType == StyleTypeIds::ElectricalConductorStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->ElectricalConductorStyles->GetById(id));
  else if (styleType == StyleTypeIds::ElectricCircuitLineStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->ElectricalCircuitLineStyles->GetById(id));
  else if (styleType == StyleTypeIds::SystemStyle)
    addEntitySubtree(pParentItem, m_pApplication->Project->SystemStyles->GetById(id));
}

void TreeViewModelBuilder::addEntitySubtree(QStandardItem * pParentItem, Renga::IEntityPtr entity)
{
  auto name = QString::fromWCharArray(entity->Name);
  auto iconPath = getRengaEntityUIData(entity->TypeId).icon16Path;
  auto itemList = createItem(name, iconPath, eTreeViewItemType::Undefined);
  pParentItem->appendRow(itemList);
}

void TreeViewModelBuilder::addSingleMaterialMaterialSubtree(
  QStandardItem* pParentItem,
  IModelObjectPtr pModelObject)
{
  QList<QStandardItem*> materialItemList = createSolidMaterialItem(pModelObject);
  addReinforcementSubtree(materialItemList.first(), pModelObject);
  pParentItem->appendRow(materialItemList);
}

void TreeViewModelBuilder::addLayersSubtree(
  QStandardItem* pParentItem,
  IModelObjectPtr pModelObject,
  int layeredMaterialId)
{
  ILayeredMaterialPtr pLayeredMaterial = getLayeredMaterial(layeredMaterialId);
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
  IModelObjectPtr pModelObject)
{
  IObjectReinforcementModelPtr pObjectReinforcementModel;

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
  IModelObjectPtr pModelObject,
  IRebarUsagePtr pRebarUsage,
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
  IModelObjectPtr pModelObject,
  IReinforcementUnitUsagePtr pReinforcementUnitUsage,
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

IMaterialPtr TreeViewModelBuilder::getMaterial(int materialId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pMaterialManager = pProject->MaterialManager;
  if (pMaterialManager == nullptr)
    return nullptr;

  return pMaterialManager->GetMaterial(materialId);
}

ILayeredMaterialPtr TreeViewModelBuilder::getLayeredMaterial(int layeredMaterialId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pLayeredMaterialManager = pProject->LayeredMaterialManager;
  if (pLayeredMaterialManager == nullptr)
    return nullptr;

  return pLayeredMaterialManager->GetLayeredMaterial(layeredMaterialId);
}

IRebarStylePtr TreeViewModelBuilder::getRebarStyle(int rebarStyleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  auto pReinforcementUnitStyleManager = pProject->ReinforcementUnitStyleManager;
  if (pReinforcementUnitStyleManager == nullptr)
    return nullptr;

  return pReinforcementUnitStyleManager->GetRebarStyle(rebarStyleId);
}

IReinforcementUnitStylePtr TreeViewModelBuilder::getReinforcementUnitStyle(int reinforcementStyleId) const
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

QList<QStandardItem*> TreeViewModelBuilder::createLevelItem(IModelObjectPtr pLevelModelObject) const
{
  bool isLevelVisible = getRengaObjectVisibility(m_pApplication, pLevelModelObject->Id);

  QList<QStandardItem*> itemList = createItem(
    QString::fromWCharArray(pLevelModelObject->Name), ":/icons/Level", eTreeViewItemType::Level, true, isLevelVisible);

  itemList.first()->setData(pLevelModelObject->Id, eTreeViewItemRole::ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createModelObjectItem(IModelObjectPtr pModelObject) const
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
  IModelObjectPtr pModelObject,
  IMaterialLayerPtr pMaterialLayer,
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

QList<QStandardItem*> TreeViewModelBuilder::createSolidMaterialItem(IModelObjectPtr pModelObject) const
{
  IObjectWithMaterialPtr pObjectWithMaterial;
  pModelObject->QueryInterface(&pObjectWithMaterial);

  bool hasMaterial = pObjectWithMaterial != nullptr && pObjectWithMaterial->HasMaterial();

  IMaterialPtr pMaterial = hasMaterial ? getMaterial(pObjectWithMaterial->MaterialId) : nullptr;

  QString materialItemName = pMaterial != nullptr ?
    QString::fromWCharArray(pMaterial->Name) :
    QApplication::translate("me_materials", "No material");

  QList<QStandardItem*> itemList = createItem(materialItemName, ":/icons/Material", eTreeViewItemType::Undefined);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createRebarUsageItem(
  IModelObjectPtr pModelObject,
  IRebarUsagePtr pRebarUsage,
  int rebarUsageIndex,
  int reinforcementUnitStyleId)
{
  IRebarStylePtr pRebarStyle = getRebarStyle(pRebarUsage->StyleId);
  assert(pRebarStyle != nullptr);

  QList<QStandardItem*> itemList = createItem(QString::fromWCharArray(pRebarStyle->Name), ":/icons/Rebar", eTreeViewItemType::RebarUsage);

  itemList.first()->setData(pModelObject->Id, eTreeViewItemRole::ModelObjectId);

  itemList.first()->setData(rebarUsageIndex, eTreeViewItemRole::RebarUsageIndex);
  itemList.first()->setData(reinforcementUnitStyleId, eTreeViewItemRole::ReinforcementUnitStyleId);

  return itemList;
}

QList<QStandardItem*> TreeViewModelBuilder::createReinforcementUnitUsageItem(
  IModelObjectPtr pModelObject,
  IReinforcementUnitUsagePtr pReinforcementUnitUsage,
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

bool TreeViewModelBuilder::objectGroupHasVisibleObject(const std::list<IModelObjectPtr>& objectsGroup) const
{
  for (auto pObject : objectsGroup)
    if (getRengaObjectVisibility(m_pApplication, pObject->Id))
      return true;
  return false;
}
