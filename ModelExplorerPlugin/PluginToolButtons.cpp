//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "PluginToolButtons.h"
#include "ModelExplorerButtonHandler.h"

#include <fstream>

#include <atlsafe.h>


static const wchar_t* c_error = L"Error";
static const wchar_t* c_openPluginIconError = L"Unable to open icon file";

PluginToolButtons::PluginToolButtons(Renga::IUIPtr pUI, const std::wstring& pluginPath) :
  m_pUI(pUI)
{
  auto pModelExplorerToolButtonAction = createModelExplorerButton(pluginPath);

  // create new buttons here

  auto pUIPanelExtension = m_pUI->CreateUIPanelExtension();
  pUIPanelExtension->AddToolButton(pModelExplorerToolButtonAction);

  // add new tool buttons to group here

  m_pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
}

Renga::IActionPtr PluginToolButtons::createModelExplorerButton(const std::wstring& pluginPath)
{
  // {A101FD63-9E61-48FA-B6DA-2B5612D695AC}
  const GUID actionId = { 0xa101fd63, 0x9e61, 0x48fa,{ 0xb6, 0xda, 0x2b, 0x56, 0x12, 0xd6, 0x95, 0xac } };

  auto pAction = m_pUI->CreateActionWithId(actionId);

  m_pModelExplorerButtonHandler.reset(new ModelExplorerButtonHandler(pAction));

  QString tooltip = QApplication::translate("ModelExplorerDialog", "Model explorer");
  pAction->ToolTip = tooltip.toStdWString().c_str();

  Renga::IImagePtr pIcon = m_pUI->CreateImage();
  if (loadIcon(pluginPath + L"\\ModelExplorerIcon.png", Renga::ImageFormat::ImageFormat_PNG, pIcon))
    pAction->Icon = pIcon;

  connect(m_pModelExplorerButtonHandler.get(), SIGNAL(buttonClicked()), this, SIGNAL(modelExplorerButtonClicked()));

  return pAction;
}

bool PluginToolButtons::loadIcon(const std::wstring& iconPath, Renga::ImageFormat iconFormat, Renga::IImagePtr& outIcon) const
{
  std::ifstream iconStream(iconPath, std::ios::in | std::ios::binary);
  if (!iconStream.is_open())
  {
    m_pUI->ShowMessageBox(Renga::MessageIcon::MessageIcon_Error, c_error, c_openPluginIconError);
    return false;
  }

  std::vector<char> buffer((
    std::istreambuf_iterator<char>(iconStream)), 
    (std::istreambuf_iterator<char>()));
  iconStream.close();

  CComSafeArray<BYTE> array(static_cast<ULONG>(buffer.size()));
  for (int i = 0; i < buffer.size(); i++)
    array[i] = buffer.at(i);

  outIcon->LoadFromData(array, iconFormat);
  return true;
}

