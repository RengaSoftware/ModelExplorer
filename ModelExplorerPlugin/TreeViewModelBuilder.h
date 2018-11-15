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
#include <comdef.h>
#include <comdef.h>
#include <comdef.h>

class TreeViewModelBuilder
{
public:
  enum Role
  {
    Role_ItemType = Qt::UserRole + 1,
    Role_ModelObjectId,
    Role_LayerIndex,
    Role_IsVisible,
    Role_RebarUsageIndex,
    Role_ReinforcementUnitUsageIndex,
    Role_ReinforcementUnitStyleId
  };

  enum ItemType
  {
    ItemType_Level = 0,
    ItemType_ModelObject,
    ItemType_ObjectGroup,
    ItemType_MaterialLayer,
    ItemType_ReinforcementUnitUsage,
    ItemType_RebarUsage,

    ItemType_Undefined = 255
  };

  TreeViewModelBuilder(Renga::IApplicationPtr pApplication);

  QStandardItemModel* build();

  static bool tryGetItemType(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);
  static bool tryGetModelObjectId(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);
  static bool tryGetLayerIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);
  static bool tryGetRebarUsageIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);
  static bool tryGetReinforcementUnitUsageIndex(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);
  static bool tryGetReinforcementUnitStyleId(QStandardItemModel* pItemModel, const QModelIndex& index, int& result);

private:
  struct ObjectTypeData
  {
    ObjectTypeData(GUID type, QString translationLiteral, QString iconPath);

    GUID m_type;
    QString m_typeNodeName;
    QString m_iconPath;
  };

  static bool tryGetIntegerData(QStandardItemModel* pItemModel, const QModelIndex& modelIndex, int role, int& result);

private:
  std::list<Renga::IModelObjectPtr> getObjectGroup(
    Renga::IModelObjectCollection& objectCollection,
    std::function<bool(Renga::IModelObject&)> groupFilter) const;
  
  std::list<Renga::IModelObjectPtr> getNonLevelObjectsWithType(
    Renga::IModelObjectCollection& objectCollection, 
    GUID objectType) const;
  
  std::list<Renga::IModelObjectPtr> getLevels(Renga::IModelObjectCollection& objects) const;
  
  std::list<Renga::IModelObjectPtr> getLevelObjectsWithType(
    Renga::IModelObjectCollection& objectCollection,
    int levelId,
    GUID objectType) const;

  const std::list<ObjectTypeData>& getLevelObjectTypeData() const;
  const std::list<ObjectTypeData>& getNonLevelObjectTypeData() const;

  void addLevelSubtree(
    QStandardItemModel* pItemModel,
    Renga::IModelObjectPtr pLevelModelObject,
    Renga::IModelObjectCollectionPtr pModelObjectCollection);

  void addObjectGroupSubtree(
    QStandardItem* pParentItem,
    const std::list<Renga::IModelObjectPtr>& objectGroup,
    const ObjectTypeData& objectTypeData);

  void addMaterialsSubtree(
    QStandardItem* pParentItem,
    Renga::IModelObjectPtr pModelObject);

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
  QList<QStandardItem*> createModelObjectItem(Renga::IModelObjectPtr pModelObject, const ObjectTypeData& objectTypeData) const;
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

private:
  Renga::IApplicationPtr m_pApplication;
};
