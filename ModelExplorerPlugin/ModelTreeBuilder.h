//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtGui/QStandardItemModel>
#include <QtCore/QTranslator.h>

#include <RengaAPI/ObjectType.h>
#include <RengaAPI/ObjectId.h>
#include <RengaAPI/ModelObject.h>
#include <RengaAPI/ModelObjectCollection.h>

class ModelTreeBuilder
{
public:
	ModelTreeBuilder(const QTranslator* pTranslator);
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
  ObjectTypeData createTypeNodeData(rengaapi::ObjectType type, QString translationLiteral, QString iconPath);

  QStandardItem* buildObjectsSubtree(
    const rengaapi::ModelObjectCollection& objCollection,     
    const ObjectTypeData& typeData, 
    rengaapi::ObjectId levelId) const;
  
  QStandardItem* createLevelObjectItem(const rengaapi::ModelObject* pLevel) const;
  QStandardItem* createItem(const QString& name, const QString& iconPath, QVariant data = QVariant()) const;
  QStandardItem* buildLevelSubtree(const rengaapi::ObjectId& levelId, const rengaapi::ModelObjectCollection& objCollection);

private:
  const QTranslator* m_pTranslator;
  std::list<ObjectTypeData> m_objectTypeDataArray;
};