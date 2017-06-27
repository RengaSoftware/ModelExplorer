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

#include <RengaAPI/ToolButtonGroup.h>
#include <RengaAPI/UIControls.h>
#include <RengaAPI/Message.h>

#include <fstream>

static const rengabase::String c_error = L"Error";
static const rengabase::String c_openPluginIconError = L"Unable to open icon file";

PluginToolButtons::PluginToolButtons(const std::wstring& pluginPath)
{
  rengaapi::ToolButton modelExplorerToolButton = createModelExplorerButton(pluginPath);
  // create new buttons here

  rengaapi::ToolButtonGroup toolButtonGroup;
  toolButtonGroup.add(modelExplorerToolButton);
  // add new tool buttons to group here

  rengaapi::UIControls::addButtonGroupToMainToolBar(toolButtonGroup);
}

rengaapi::ToolButton PluginToolButtons::createModelExplorerButton(const std::wstring& pluginPath)
{
  m_pModelExplorerButtonHandler.reset(new ModelExplorerButtonHandler());
  
  rengaapi::ToolButton toolButton;
  toolButton.setClickHandler(m_pModelExplorerButtonHandler.get());

  QString tooltip = QApplication::translate("ModelExplorerDialog", "Model explorer");
  toolButton.setToolTip(rengabase::rengaStringFromStdWString(tooltip.toStdWString()));

  rengaapi::Image icon;
  if(loadIcon(pluginPath + L"\\ModelExplorerIcon.png", rengaapi::Image::Format::PNG, icon))
    toolButton.setImage(icon);

  connect(m_pModelExplorerButtonHandler.get(), SIGNAL(buttonClicked()), this, SIGNAL(modelExplorerButtonClicked()));

  return toolButton;
}

bool PluginToolButtons::loadIcon(const std::wstring& iconPath, rengaapi::Image::Format iconFormat, rengaapi::Image& outIcon) const
{
  std::ifstream iconStream(iconPath, std::ios::in | std::ios::binary);
  if (!iconStream.is_open())
  {
    rengaapi::Message::showMessageBox(rengaapi::Message::Error, c_error, c_openPluginIconError);
    return false;
  }

  std::vector<char> buffer((
    std::istreambuf_iterator<char>(iconStream)), 
    (std::istreambuf_iterator<char>()));
  iconStream.close();

  outIcon.loadFromData(buffer.data(), buffer.size(), iconFormat);
  return true;
}

