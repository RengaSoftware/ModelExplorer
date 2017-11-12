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

#include <RengaAPI/Beam.h>
#include <RengaAPI/Column.h>
#include <RengaAPI/Door.h>
#include <RengaAPI/Floor.h>
#include <RengaAPI/IsolatedFoundation.h>
#include <RengaAPI/Level.h>
#include <RengaAPI/Model.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Opening.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/Railing.h>
#include <RengaAPI/Ramp.h>
#include <RengaAPI/Roof.h>
#include <RengaAPI/Room.h>
#include <RengaAPI/Stair.h>
#include <RengaAPI/Wall.h>
#include <RengaAPI/WallFoundation.h>
#include <RengaAPI/Window.h>

int ModelTreeBuilder::objectIDRole = Qt::UserRole + 1;

ModelTreeBuilder::ObjectTypeData::ObjectTypeData(rengaapi::ObjectType type, QString translationLiteral, QString iconPath)
  : m_type(type)
  , m_typeNodeName(translationLiteral)
  , m_iconPath(iconPath)
{}

ModelTreeBuilder::ModelTreeBuilder()
{
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::WallType, QApplication::translate("me_modelObjects", "Walls"), ":/icons/Wall"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::ColumnType, QApplication::translate("me_modelObjects", "Columns"), ":/icons/Column"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::FloorType, QApplication::translate("me_modelObjects", "Floors"), ":/icons/Floor"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::OpeningType, QApplication::translate("me_modelObjects", "Openings"), ":/icons/Opening"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::RoofType, QApplication::translate("me_modelObjects", "Roofs"), ":/icons/Roof"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::BeamType, QApplication::translate("me_modelObjects", "Beams"), ":/icons/Beam"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::StairType, QApplication::translate("me_modelObjects", "Stairs"), ":/icons/Stair"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::RampType, QApplication::translate("me_modelObjects", "Ramps"), ":/icons/Ramp"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::WindowType, QApplication::translate("me_modelObjects", "Windows"), ":/icons/Window"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::DoorType, QApplication::translate("me_modelObjects", "Doors"), ":/icons/Door"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::RailingType, QApplication::translate("me_modelObjects", "Railings"), ":/icons/Railing"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::RoomType, QApplication::translate("me_modelObjects", "Rooms"), ":/icons/Room"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::IsolatedFoundationType, QApplication::translate("me_modelObjects", "Isolated foundations"), ":/icons/Isolated_foundation"));
  m_objectTypeDataArray.push_back( ObjectTypeData(rengaapi::ModelObjectTypes::WallFoundationType, QApplication::translate("me_modelObjects", "Wall foundations"), ":/icons/Wall_foundation"));
}

QStandardItemModel* ModelTreeBuilder::buildModelTree()
{
  QStandardItemModel* pResultModel = new QStandardItemModel();
  rengaapi::Model rengaProjectModel = rengaapi::Project::model();

  rengaapi::ObjectFilter levelsFilter = rengaapi::ObjectFilter::createObjectFilterByType(rengaapi::ModelObjectTypes::LevelType);
  rengaapi::ModelObjectCollection levelsCollection = rengaProjectModel.objects(levelsFilter);

  // get level objects
  std::list<rengaapi::Level*> levels;
  for (auto pObj : levelsCollection)
  {
    rengaapi::Level* pLevelObject = dynamic_cast<rengaapi::Level*>(pObj);
    assert(pLevelObject != nullptr);
    levels.push_back(pLevelObject);
  }

  levels.sort([=](const rengaapi::Level* pLevel1, const rengaapi::Level* pLevel2)
  {
    return pLevel1->elevation().inMillimeters() < pLevel2->elevation().inMillimeters();
  });

  // build subtree for each level
  rengaapi::ModelObjectCollection objCollection = rengaProjectModel.objects();
  for (auto pLevel : levels)
  {
    QList<QStandardItem*> pLevelItem = buildLevelSubtree(pLevel->objectId(), objCollection);
    pResultModel->appendRow(pLevelItem);
  }

  return pResultModel;
}

QList<QStandardItem*> ModelTreeBuilder::createLevelObjectItem(const rengaapi::ModelObject* pObject) const
{
  assert(pObject != nullptr);
  assert(pObject->type() == rengaapi::ModelObjectTypes::LevelType);

  QString levelNodeName = rengaStringToQString(pObject->name());
  bool isVisible = getRengaObjectVisibility(pObject->objectId());
  QVariant levelNodeData(pObject->objectId().id());

  return createItem(levelNodeName, QString(":/icons/Level"), isVisible, levelNodeData);
}

QList<QStandardItem*> ModelTreeBuilder::createItem(const QString& name, const QString& iconPath, bool isVisible, QVariant data) const
{
  QStandardItem* pModelObjectItem = new QStandardItem(name);
  pModelObjectItem->setIcon(QIcon(iconPath));
  if(!data.isNull())
    pModelObjectItem->setData(data, ModelTreeBuilder::objectIDRole);

  QStandardItem* pModelObjectVisibilityItem = new QStandardItem();
  pModelObjectVisibilityItem->setIcon(QIcon(QString(isVisible ? ":/icons/Visible" : ":/icons/Hidden")));
  pModelObjectVisibilityItem->setData(QVariant(isVisible), ModelTreeBuilder::objectIDRole);

  QList<QStandardItem*> itemList;
  itemList.append(pModelObjectItem);
  itemList.append(pModelObjectVisibilityItem);
  return itemList;
}

QList<QStandardItem*> ModelTreeBuilder::buildLevelSubtree(const rengaapi::ObjectId& levelId, const rengaapi::ModelObjectCollection& objCollection)
{
  QList<QStandardItem*> pLevelItem = createLevelObjectItem(objCollection.get(levelId));

  for(auto objectTypeData : m_objectTypeDataArray)
    pLevelItem.at(0)->appendRow( buildObjectsSubtree(objCollection, objectTypeData, levelId) );

  return pLevelItem;
}

QList<QStandardItem*> ModelTreeBuilder::buildObjectsSubtree(const rengaapi::ModelObjectCollection& objCollection, 
                                                            const ObjectTypeData& typeData, 
                                                            rengaapi::ObjectId levelId) const
{
  QList<QStandardItem*> pObjectsByTypeAndLevelSubtree = createItem(typeData.m_typeNodeName, QString(":/icons/Folder"), !false); // TODO: remove ! after bugfix ¹21933
  bool isVisible = false;

  for (auto pObject : objCollection)
  {
    assert(pObject != nullptr);
    if(pObject->type() != typeData.m_type)
      continue;

    if (pObject->type() == rengaapi::ModelObjectTypes::WallType)
    {
      rengaapi::Wall* pWall = dynamic_cast<rengaapi::Wall*>(pObject);

      if(pWall->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::ColumnType)
    {
      rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);

      if(pColumn->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::FloorType)
    {
      rengaapi::Floor* pFloor = dynamic_cast<rengaapi::Floor*>(pObject);

      if(pFloor->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::OpeningType)
    {
      rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);

      if(pOpening->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::RoofType)
    {
      rengaapi::Roof* pRoof = dynamic_cast<rengaapi::Roof*>(pObject);

      if(pRoof->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::BeamType)
    {
      rengaapi::Beam* pBeamObject = dynamic_cast<rengaapi::Beam*>(pObject);

      if(pBeamObject->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::StairType)
    {
      rengaapi::Stair* pStair = dynamic_cast<rengaapi::Stair*>(pObject);

      if(pStair->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::RampType)
    {
      rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);

      if(pRamp->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::WindowType)
    {
      rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);

      if(pWindow->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::DoorType)
    {
      rengaapi::Door* pDoor = dynamic_cast<rengaapi::Door*>(pObject);

      if(pDoor->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::RailingType)
    {
      rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);

      if(pRailing->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::RoomType)
    {
      rengaapi::Room* pRoom = dynamic_cast<rengaapi::Room*>(pObject);

      if(pRoom->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::IsolatedFoundationType)
    {
      rengaapi::IsolatedFoundation* pIsolatedFoundation = dynamic_cast<rengaapi::IsolatedFoundation*>(pObject);

      if(pIsolatedFoundation->levelId() != levelId)
        continue;
    }
    else if (pObject->type() == rengaapi::ModelObjectTypes::WallFoundationType)
    {
      rengaapi::WallFoundation* pWallFoundation = dynamic_cast<rengaapi::WallFoundation*>(pObject);

      if(pWallFoundation->levelId() != levelId)
        continue;
    }
    else
    {
      continue;
    }

    QString objectItemName = rengaStringToQString(pObject->name());
    QString objectItemIconPath(typeData.m_iconPath);
    bool isChildVisible = getRengaObjectVisibility(pObject->objectId());
    QVariant objectItemData(pObject->objectId().id());

    QList<QStandardItem*> pObjectItem = createItem(objectItemName, objectItemIconPath, isChildVisible, objectItemData);
    pObjectsByTypeAndLevelSubtree.at(0)->appendRow(pObjectItem);
    isVisible |= isChildVisible;
  }

  if (pObjectsByTypeAndLevelSubtree.at(0)->rowCount() == 0)
  {
    // get level visibility
    isVisible = getRengaObjectVisibility(levelId);
  }

  QStandardItem* icon = pObjectsByTypeAndLevelSubtree.back();
  icon->setIcon(QIcon(isVisible ? ":/icons/Visible" : ":/icons/Hidden"));
  icon->setData(QVariant(isVisible), ModelTreeBuilder::objectIDRole);

  return pObjectsByTypeAndLevelSubtree;
}
