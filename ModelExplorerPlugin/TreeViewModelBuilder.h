//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtGui/QStandardItemModel>

#include <comdef.h>


struct GuidComparator
{
  bool operator () (const GUID& left, const GUID& right) const
  {
    return memcmp(&left, &right, sizeof(GUID)) < 0;
  }
};

struct LevelObjectGroup
{
  int levelId;
  GUID objectType;

  LevelObjectGroup(int levelId, GUID objectType)
    : levelId(levelId), objectType(objectType)
  {
  }

  bool operator < (const LevelObjectGroup& other) const
  {
    if (levelId != other.levelId)
      return levelId < other.levelId;

    return GuidComparator()(objectType, other.objectType);
  }
};

class TreeViewModelBuilder
{
public:
  TreeViewModelBuilder(Renga::IApplicationPtr pApplication);
  QStandardItemModel* build();

private:
  void processModelObjectCollection(Renga::IModelObjectCollectionPtr pModelObjectCollection);
  void processLevelObject(Renga::IModelObjectPtr pModelObject);
  void processNonLevelObject(Renga::IModelObjectPtr pModelObject);

  void addNonLevelSubtree(
    QStandardItemModel* pItemModel,
    Renga::IModelObjectCollectionPtr pModelObjectCollection);

  void addLevelSubtree(
    QStandardItemModel* pItemModel,
    Renga::IModelObjectPtr pLevelModelObject,
    Renga::IModelObjectCollectionPtr pModelObjectCollection);

  void addObjectGroupSubtree(
    QStandardItem* pParentItem,
    const std::list<Renga::IModelObjectPtr>& objectGroup,
    const QString groupName);

  void addObjectSubtree(QStandardItem* pParentItem, Renga::IModelObjectPtr pObject);

  void addStyleSubtree(
      QStandardItem* pParentItem,
      Renga::IModelObjectPtr pModelObject,
      GUID styleType, // Replace with Renga schema
      int id /*TODO: should be Renga::IEntity in future*/);
  
  void addSingleMaterialMaterialSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject);

  void addLayersSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject,
    int layeredMaterialId);

  void addReinforcementSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject);

  void addRebarUsageSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject,
    Renga::IRebarUsagePtr pRebarUsage,
    int rebarUsageIndex,
    int reinforcementUnitStyleId);

  void addReinforcementUnitUsageSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject,
    Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage, int reinforcementUnitUsageIndex);

  Renga::IMaterialPtr getMaterial(int materialId) const;
  Renga::ILayeredMaterialPtr getLayeredMaterial(int layeredMaterialId) const;
  Renga::IRebarStylePtr getRebarStyle(int rebarStyleId) const;
  Renga::IReinforcementUnitStylePtr getReinforcementUnitStyle(int reinforcementUnitStyleId) const;

  QList<QStandardItem*> createLevelItem(Renga::IModelObjectPtr pLevelModelObject) const;
  QList<QStandardItem*> createOtherGroupItem() const;
  QList<QStandardItem*> createModelObjectItem(Renga::IModelObjectPtr pModelObject) const;
  QList<QStandardItem*> createMaterialLayerItem(
    Renga::IModelObjectPtr pModelObject,
    Renga::IMaterialLayerPtr pMaterialLayer,
    int layerIndex) const;
  QList<QStandardItem*> createSolidMaterialItem(
    Renga::IModelObjectPtr pModelObject) const;
  QList<QStandardItem*> createRebarUsageItem(
    Renga::IModelObjectPtr pModelObject,
    Renga::IRebarUsagePtr pRebarUsage,
    int rebarUsageIndex,
    int reinforcementUnitStyleId);
  QList<QStandardItem*> createReinforcementUnitUsageItem(
    Renga::IModelObjectPtr pModelObject,
    Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsagePtr,
    int reinforcementUnitUsageIndex);

  QList<QStandardItem*> createItem(
    const QString& name,
    const QString& iconPath,
    int itemType,
    bool createVisibilityItem = false,
    bool isVisible = true) const;

  void setItemVisibilityState(QList<QStandardItem*>& itemList, bool isVisible) const;
  bool objectGroupHasVisibleObject(const std::list<Renga::IModelObjectPtr>& objectsGroup) const;
  const std::map<GUID, GUID>& parameterIdToEntityTypeDict() const;

private:
  Renga::IApplicationPtr m_pApplication;

  // TODO: remove these members
  std::list<Renga::IModelObjectPtr> m_levels;
  std::map<LevelObjectGroup, std::list<Renga::IModelObjectPtr>> m_levelObjects;
  std::map<GUID, std::list<Renga::IModelObjectPtr>, GuidComparator> m_nonLevelObjects;
};
