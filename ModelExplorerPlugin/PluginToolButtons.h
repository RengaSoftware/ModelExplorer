//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtCore/QObject>
#include <QtCore/QTranslator>

#include <memory>

#include <RengaAPI/Image.h>
#include <RengaAPI/Toolbutton.h>

class ModelExplorerButtonHandler;

class PluginToolButtons : public QObject
{
  Q_OBJECT

public:
  PluginToolButtons(const std::wstring& pluginPath, const QTranslator* pTranslator);

signals:
  void modelExplorerButtonClicked();

private:
  rengaapi::ToolButton createModelExplorerButton(const std::wstring& pluginPath);
  bool loadIcon(const std::wstring& iconPath, rengaapi::Image::Format iconFormat, rengaapi::Image& outIcon) const;

private:
  const QTranslator* m_pTranslator;
  std::unique_ptr<ModelExplorerButtonHandler> m_pModelExplorerButtonHandler;
};