//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtGui/QStandardItemModel>

#include <RengaAPI/ObjectType.h>
#include <RengaAPI/ObjectId.h>
#include <RengaAPI/ModelObject.h>
#include <RengaAPI/ModelObjectCollection.h>

class ModelTreeBuilder
{
public:
  ModelTreeBuilder();
  QStandardItemModel* buildModelTree();

  static int objectIDRole;

private:
  struct ObjectTypeData
  {
    ObjectTypeData(rengaapi::ObjectType type, QString translationLiteral, QString iconPath);

    rengaapi::ObjectType m_type;
    QString m_typeNodeName;
    QString m_iconPath;
  };

private:
  QList<QStandardItem*> buildObjectsSubtree(
    const rengaapi::ModelObjectCollection& objCollection,     
    const ObjectTypeData& typeData, 
    rengaapi::ObjectId levelId) const;

  QList<QStandardItem*> createLevelObjectItem(const rengaapi::ModelObject* pLevel) const;
  QList<QStandardItem*> createItem(const QString& name, const QString& iconPath, bool isVisible, QVariant data = QVariant()) const;
  QList<QStandardItem*> buildLevelSubtree(const rengaapi::ObjectId& levelId, const rengaapi::ModelObjectCollection& objCollection);

private:
  std::list<ObjectTypeData> m_objectTypeDataArray;
};