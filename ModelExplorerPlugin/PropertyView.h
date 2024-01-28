//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyManager.h"

#include <qttreepropertybrowser.h>


class IPropertyViewBuilder;
class RengaPropertyController;

class PropertyView : public QtTreePropertyBrowser
{
  Q_OBJECT
    
public:
  enum Mode
  {
    CategoryMode = 0,
    ListMode = 1,
  };

  PropertyView(QWidget* pParent);
  void showProperties(
      const IPropertyViewBuilder& builder,
      std::unique_ptr<RengaPropertyController> pPropertyController);
  void changeMode(PropertyView::Mode newMode);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManager m_quantitiesMng;
  PropertyManager m_parametersMng;
  PropertyManager m_propertiesMng;
  Mode m_propertyViewMode;
  std::unique_ptr<RengaPropertyController> m_pPropertyController;
};
