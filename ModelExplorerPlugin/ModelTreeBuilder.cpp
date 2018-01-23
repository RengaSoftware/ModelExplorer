//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ModelTreeBuilder.h"
#include "RengaObjectVisibility.h"

#include <Renga/ObjectTypes.h>


int ModelTreeBuilder::objectIDRole = Qt::UserRole + 1;

ModelTreeBuilder::ObjectTypeData::ObjectTypeData(GUID type, QString translationLiteral, QString iconPath) :
  m_type(type),
  m_typeNodeName(translationLiteral),
  m_iconPath(iconPath)
{
}

ModelTreeBuilder::ModelTreeBuilder(Renga::IApplicationPtr pApplication) :
  m_pApplication(pApplication)
{
  add(Renga::ObjectTypes::Wall, QApplication::translate("me_modelObjects", "Walls"), ":/icons/Wall");
  add(Renga::ObjectTypes::Column, QApplication::translate("me_modelObjects", "Columns"), ":/icons/Column");
  add(Renga::ObjectTypes::Floor, QApplication::translate("me_modelObjects", "Floors"), ":/icons/Floor");
  add(Renga::ObjectTypes::Opening, QApplication::translate("me_modelObjects", "Openings"), ":/icons/Opening");
  add(Renga::ObjectTypes::Roof, QApplication::translate("me_modelObjects", "Roofs"), ":/icons/Roof");
  add(Renga::ObjectTypes::Beam, QApplication::translate("me_modelObjects", "Beams"), ":/icons/Beam");
  add(Renga::ObjectTypes::Stair, QApplication::translate("me_modelObjects", "Stairs"), ":/icons/Stair");
  add(Renga::ObjectTypes::Ramp, QApplication::translate("me_modelObjects", "Ramps"), ":/icons/Ramp");
  add(Renga::ObjectTypes::Window, QApplication::translate("me_modelObjects", "Windows"), ":/icons/Window");
  add(Renga::ObjectTypes::Door, QApplication::translate("me_modelObjects", "Doors"), ":/icons/Door");
  add(Renga::ObjectTypes::Railing, QApplication::translate("me_modelObjects", "Railings"), ":/icons/Railing");
  add(Renga::ObjectTypes::Room, QApplication::translate("me_modelObjects", "Rooms"), ":/icons/Room");
  add(Renga::ObjectTypes::IsolatedFoundation, QApplication::translate("me_modelObjects", "Isolated foundations"), ":/icons/Isolated_foundation");
  add(Renga::ObjectTypes::WallFoundation, QApplication::translate("me_modelObjects", "Wall foundations"), ":/icons/Wall_foundation");
  add(Renga::ObjectTypes::AssemblyInstance, QApplication::translate("me_modelObjects", "Assembly instances"), ":/icons/Wall_foundation");
}

void ModelTreeBuilder::add(GUID type, const QString& translationLiteral, QString iconPath)
{
  m_objectTypeDataArray.push_back(ObjectTypeData(type, translationLiteral, iconPath));
}

QStandardItemModel* ModelTreeBuilder::buildModelTree()
{
  QStandardItemModel* pResultModel = new QStandardItemModel();

  auto pProject = m_pApplication->GetProject();
  auto pProjectModel = pProject->GetModel();
  auto pModelObjectCollection = pProjectModel->GetObjects();

  // get level objects
  std::list<Renga::ILevelPtr> levels;
  for (int i = 0; i < pModelObjectCollection->GetObjectCount(); ++i)
  {
    auto pModelObject = pModelObjectCollection->GetObjectByIndex(i);
    if (pModelObject->GetObjectType() != Renga::ObjectTypes::Level)
      continue;

    Renga::ILevelPtr pLevelObject;
    pModelObject->QueryInterface(&pLevelObject);

    assert(pLevelObject);

    levels.push_back(pLevelObject);
  }

  levels.sort([=](const Renga::ILevelPtr pLevel1, const Renga::ILevelPtr pLevel2)
  {
    return pLevel1->GetElevation() < pLevel2->GetElevation();
  });

  // build subtree for each level
  for (auto pLevel : levels)
  {
    QList<QStandardItem*> pLevelItem = buildLevelSubtree(pLevel->GetId(), pModelObjectCollection);
    pResultModel->appendRow(pLevelItem);
  }

  return pResultModel;
}

QList<QStandardItem*> ModelTreeBuilder::createLevelObjectItem(Renga::IModelObjectPtr pObject) const
{
  assert(pObject);
  assert(pObject->GetObjectType() == Renga::ObjectTypes::Level);

  const auto id = pObject->GetId();

  QString levelNodeName = QString::fromWCharArray(pObject->GetName());
  bool isVisible = getRengaObjectVisibility(m_pApplication, id);
  QVariant levelNodeData(id);

  return createItem(levelNodeName, QString(":/icons/Level"), isVisible, levelNodeData);
}

QList<QStandardItem*> ModelTreeBuilder::createItem(const QString& name, const QString& iconPath, bool isVisible, QVariant data) const
{
  auto pModelObjectItem = new QStandardItem(name);
  pModelObjectItem->setIcon(QIcon(iconPath));
  if(!data.isNull())
    pModelObjectItem->setData(data, ModelTreeBuilder::objectIDRole);

  auto pModelObjectVisibilityItem = new QStandardItem();
  pModelObjectVisibilityItem->setIcon(QIcon(QString(isVisible ? ":/icons/Visible" : ":/icons/Hidden")));
  pModelObjectVisibilityItem->setData(QVariant(isVisible), ModelTreeBuilder::objectIDRole);

  QList<QStandardItem*> itemList;
  itemList.append(pModelObjectItem);
  itemList.append(pModelObjectVisibilityItem);
  return itemList;
}

QList<QStandardItem*> ModelTreeBuilder::buildLevelSubtree(const int levelId, const Renga::IModelObjectCollectionPtr objCollection)
{
  QList<QStandardItem*> pLevelItem = createLevelObjectItem(objCollection->GetObjectById(levelId));

  for(auto objectTypeData : m_objectTypeDataArray)
    pLevelItem.at(0)->appendRow( buildObjectsSubtree(objCollection, objectTypeData, levelId) );

  return pLevelItem;
}

QList<QStandardItem*> ModelTreeBuilder::buildObjectsSubtree(const Renga::IModelObjectCollectionPtr objCollection, 
                                                            const ObjectTypeData& typeData, 
                                                            int levelId) const
{
  QList<QStandardItem*> pObjectsByTypeAndLevelSubtree = createItem(typeData.m_typeNodeName, QString(":/icons/Folder"), !false); // TODO: remove ! after bugfix ¹21933
  bool isVisible = false;

  for (int i = 0; i < objCollection->GetObjectCount(); ++i)
  {
    auto pObject = objCollection->GetObjectByIndex(i);

    assert(pObject != nullptr);
    if (pObject->GetObjectType() != typeData.m_type)
      continue;

    Renga::ILevelObjectPtr pLevelObject;
    pObject->QueryInterface(&pLevelObject);
    if (!pLevelObject)
      continue;

    if (pLevelObject->GetLevelId() != levelId)
      continue;

    const auto id = pObject->GetId();

    QString objectItemName = QString::fromWCharArray(pObject->GetName());
    QString objectItemIconPath(typeData.m_iconPath);
    bool isChildVisible = getRengaObjectVisibility(m_pApplication, id);
    QVariant objectItemData(id);

    QList<QStandardItem*> pObjectItem = createItem(objectItemName, objectItemIconPath, isChildVisible, objectItemData);
    pObjectsByTypeAndLevelSubtree.at(0)->appendRow(pObjectItem);
    isVisible |= isChildVisible;
  }

  if (pObjectsByTypeAndLevelSubtree.at(0)->rowCount() == 0)
  {
    // get level visibility
    isVisible = getRengaObjectVisibility(m_pApplication, levelId);
  }

  QStandardItem* icon = pObjectsByTypeAndLevelSubtree.back();
  icon->setIcon(QIcon(isVisible ? ":/icons/Visible" : ":/icons/Hidden"));
  icon->setData(QVariant(isVisible), ModelTreeBuilder::objectIDRole);

  return pObjectsByTypeAndLevelSubtree;
}
