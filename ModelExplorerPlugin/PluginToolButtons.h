//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtCore/QObject>


class ModelExplorerButtonHandler;

class PluginToolButtons : public QObject
{
  Q_OBJECT

public:
  PluginToolButtons(Renga::IUIPtr pUI, const std::wstring& pluginPath);

signals:
  void modelExplorerButtonClicked();

private:
  Renga::IActionPtr createModelExplorerButton(const std::wstring& pluginPath);
  bool loadIcon(const std::wstring& iconPath, Renga::ImageFormat iconFormat, Renga::IImagePtr& outIcon) const;

private:
  const QTranslator* m_pTranslator;
  std::unique_ptr<ModelExplorerButtonHandler> m_pModelExplorerButtonHandler;
  Renga::IUIPtr m_pUI;
};