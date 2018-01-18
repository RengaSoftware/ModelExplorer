//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtGui/QStandardItemModel>


class ModelTreeBuilder
{
public:
  ModelTreeBuilder(Renga::IApplicationPtr pApplication);
  QStandardItemModel* buildModelTree();

  static int objectIDRole;

private:
  struct ObjectTypeData
  {
    ObjectTypeData(GUID type, QString translationLiteral, QString iconPath);

    GUID m_type;
    QString m_typeNodeName;
    QString m_iconPath;
  };

private:
  void add(GUID type, const QString& translationLiteral, QString iconPath);

  QList<QStandardItem*> buildObjectsSubtree(
    const Renga::IModelObjectCollectionPtr objCollection,
    const ObjectTypeData& typeData, 
    int levelId) const;

  QList<QStandardItem*> createLevelObjectItem(Renga::IModelObjectPtr pLevel) const;
  QList<QStandardItem*> createItem(const QString& name, const QString& iconPath, bool isVisible, QVariant data = QVariant()) const;
  QList<QStandardItem*> buildLevelSubtree(const int levelId, const Renga::IModelObjectCollectionPtr objCollection);

private:
  Renga::IApplicationPtr m_pApplication;
  std::list<ObjectTypeData> m_objectTypeDataArray;
};