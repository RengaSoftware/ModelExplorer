//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ModelExplorerWidget.h"

#include <windows.h>

#include "ui_ModelExplorer.h"

#include <QtCore/QFile.h>
#include <QtWidgets/QButtonGroup.h>

#include <RengaAPI/Application.h>

static const unsigned int c_displacementFromParentTop = 100;
static const unsigned int c_displacementFromParentLeft = 5;

ModelExplorerWidget::ModelExplorerWidget()
  : QWidget(nullptr, Qt::Tool)
{
  m_pUi.reset(new Ui::ModelExplorerDialog());
  m_pUi->setupUi(this);

  // load main window stylesheet
  QFile styleFile(":/styles/mainFormStyle.qss");
  if(styleFile.open(QIODevice::ReadOnly))
    setStyleSheet(styleFile.readAll());

  // load UI
  QPushButton* pRefreshButton = createPushButton(":/icons/Refresh.png", QApplication::translate("me_propertyView", "refresh"));
  connect(pRefreshButton, SIGNAL(clicked()), this, SIGNAL(rebuildModelTree()));

  QPushButton* pShowButton = createPushButton(":/icons/Visible.png", QApplication::translate("me_propertyView", "visible"));
  QPushButton* pHideButton = createPushButton(":/icons/Hidden.png", QApplication::translate("me_propertyView", "hidden"));

  // top toolbar
  QToolBar* pTopToolBar = createToolBar(m_pUi->layoutWidget);
  pTopToolBar->addWidget(pRefreshButton);
  pTopToolBar->addWidget(pShowButton);
  pTopToolBar->addWidget(pHideButton);
  m_pUi->topVerticalLayout->addWidget(pTopToolBar);

  // tree view
  createModelTreeView();
  connect(pShowButton, SIGNAL(clicked()), m_pModelTreeView, SLOT(showSelectedItem()));
  connect(pHideButton, SIGNAL(clicked()), m_pModelTreeView, SLOT(hideSelectedItem()));
  m_pUi->topVerticalLayout->addWidget(m_pModelTreeView);

  // view mode buttons
  QPushButton* pCategoryButton = createPushButton(":/icons/Category.png", QApplication::translate("me_propertyView", "category"));
  pCategoryButton->setCheckable(true);
  pCategoryButton->setChecked(true);

  QPushButton* pListButton = createPushButton(":/icons/List.png", QApplication::translate("me_propertyView", "list"));
  pListButton->setCheckable(true);

  // bottom toolbar
  QToolBar* pBottomToolBar = createToolBar(m_pUi->layoutWidget2);
  pBottomToolBar->addWidget(pCategoryButton);
  pBottomToolBar->addWidget(pListButton);
  m_pUi->bottomVerticalLayout->addWidget(pBottomToolBar);

  // view mode button group
  QButtonGroup* pButtonGroup = new QButtonGroup(pBottomToolBar);
  pButtonGroup->setExclusive(true);
  pButtonGroup->addButton(pCategoryButton, 0);
  pButtonGroup->addButton(pListButton, 1);
  connect(pButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=](int id){setPropertyViewMode(id);});

  createPropertyView();
  m_pUi->bottomVerticalLayout->addWidget(m_pPropertyView);
}

ModelExplorerWidget::~ModelExplorerWidget()
{}

void ModelExplorerWidget::setPropertyViewMode(int pressedButtonId)
{
  m_pPropertyView->changeMode(ObjectPropertyView::Mode(pressedButtonId));
}

void ModelExplorerWidget::onObjectSelectedInTree(const rengaapi::ObjectId& id)
{
  m_pPropertyView->setSelectedObjectId(id);
}

QPushButton* ModelExplorerWidget::createPushButton(const QString& iconPath, const QString& tooltip)
{
  QIcon buttonIcon;
  buttonIcon.addFile(iconPath, QSize(16, 16), QIcon::Normal, QIcon::Off);

  QPushButton* pButton = new QPushButton(buttonIcon, QString());
  pButton->setMinimumSize(QSize(24, 24));
  pButton->setMaximumSize(QSize(24, 24));
  pButton->setCursor(QCursor(Qt::ArrowCursor));

  // Note: Focus does not go to all the buttons in QButtonGroup
  // https://bugreports.qt.io/browse/QTBUG-52036
  pButton->setFocusPolicy(Qt::StrongFocus);
  pButton->setToolTip(tooltip);
  return pButton;
}

QToolBar* ModelExplorerWidget::createToolBar(QWidget* parentWidget)
{
  QToolBar* pToolBar = new QToolBar(parentWidget);
  pToolBar->setMovable(false);
  pToolBar->setAllowedAreas(Qt::TopToolBarArea);
  pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  pToolBar->setFloatable(false);
  pToolBar->setFocusPolicy(Qt::NoFocus);
  return pToolBar;
}

void ModelExplorerWidget::createModelTreeView()
{
  m_pModelTreeView = new ModelTreeView(m_pUi->layoutWidget);
  m_pModelTreeView->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
  m_pModelTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_pModelTreeView->setHeaderHidden(true);

  connect(this, SIGNAL(rebuildModelTree()), m_pModelTreeView, SLOT(onRebuildTree()));
  connect(m_pModelTreeView, SIGNAL(modelObjectSelectionChanged(const rengaapi::ObjectId&)), 
    this, SLOT(onObjectSelectedInTree(const rengaapi::ObjectId&)));
}

void ModelExplorerWidget::createPropertyView()
{
  m_pPropertyView = new ObjectPropertyView(this);
  m_pPropertyView->setResizeMode(QtTreePropertyBrowser::Interactive);
  m_pPropertyView->setHeaderVisible(true);
  m_pPropertyView->setAlternatingRowColors(false);
  m_pPropertyView->setSplitterPosition(150);
}

void ModelExplorerWidget::updatePlacement()
{
  auto rengaMainWindowHwnd = (HWND) rengaapi::Application::topLevelWindow();

  RECT parentWindowRect;
  GetWindowRect(rengaMainWindowHwnd, &parentWindowRect);

  move(parentWindowRect.left + c_displacementFromParentLeft, parentWindowRect.top + c_displacementFromParentTop);
}

void ModelExplorerWidget::updateOwner()
{
  auto pluginWindowHwnd = (HWND) this->winId();
  auto rengaMainWindowHwnd = (HWND) rengaapi::Application::topLevelWindow();
  SetWindowLongPtr(pluginWindowHwnd, GWLP_HWNDPARENT, (LONG_PTR)rengaMainWindowHwnd);
}

void ModelExplorerWidget::readModelAndShow()
{
  emit rebuildModelTree();

  updatePlacement();
  show();
  activateWindow();
  updateOwner();
}
