//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "ModelTreeView.h"
#include "ObjectPropertyView.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>

#include <RengaAPI/ObjectId.h>

namespace Ui
{
  class ModelExplorerDialog;
}

class ModelExplorerWidget : public QWidget
{
  Q_OBJECT

public:
  ModelExplorerWidget();
  ~ModelExplorerWidget();

public:
  void readModelAndShow();

signals:
  void rebuildModelTree();

private slots:
  void setPropertyViewMode(int buttonId);
  void onObjectSelectedInTree(const rengaapi::ObjectId& id);

private:
  QPushButton* createPushButton(const QString& iconPath, const QString& tooltip);
  QToolBar* createToolBar(QWidget* parentWidget);
  void createModelTreeView();
  void createPropertyView();
  void updatePlacement();
  void updateOwner();

private:
  std::unique_ptr<Ui::ModelExplorerDialog> m_pUi;
  ModelTreeView* m_pModelTreeView;
  ObjectPropertyView* m_pPropertyView;
};
