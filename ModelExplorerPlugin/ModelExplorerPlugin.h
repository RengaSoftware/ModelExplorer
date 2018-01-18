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

#include <Renga/IPlugin.h>


class PluginToolButtons;
class RengaEventsHandler;
class ModelExplorerWidget;

class ModelExplorerPlugin : public QObject, public plugins::IPlugin
{
  Q_OBJECT

public:
  ModelExplorerPlugin();
  ~ModelExplorerPlugin();

  // plugins::IPlugin
  bool initialize(const wchar_t* pluginPath);
  void stop();

private slots:
  void onModelExplorerButtonClicked();
  void onProjectAboutToClose();

private:
  void addPluginButtons(const std::wstring& pluginPath);
  void subscribeOnRengaEvents();
  bool loadTranslator(const std::wstring& pluginPath);
  QString translationFileName();

private:
  QTranslator m_translator;

  Renga::IApplicationPtr m_pApplication;
  Renga::IUIPtr m_pUI;

#ifdef _DEBUG
  // NOTE: To debug plugin you should create debug QApplication in plugin.
  std::unique_ptr<QApplication> m_pApp;
#endif

  std::unique_ptr<PluginToolButtons> m_pPluginToolButtons;
  std::unique_ptr<RengaEventsHandler> m_pRengaEventsHandler;
  std::unique_ptr<ModelExplorerWidget> m_pWidget;
};