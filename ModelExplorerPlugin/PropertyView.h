//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "IPropertyViewBuilder.h"
#include "PropertyManager.h"
#include "RengaModelUtils.h"
#include "RengaPropertyController.h"

#include <qttreepropertybrowser.h>


class IPropertyViewBuilder;

class PropertyView : public QtTreePropertyBrowser
{
  Q_OBJECT
    
public:
  enum Mode
  {
    CategoryMode = 0,
    ListMode = 1,
  };

  PropertyView(QWidget* pParent, Renga::IApplicationPtr pApplication);
  void showProperties(const IPropertyViewBuilder& builder, PropertyContainerAccess propertiesAccess);
  void changeMode(PropertyView::Mode newMode);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  
  Renga::IApplicationPtr m_pRenga;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManager m_quantitiesMng;
  PropertyManager m_parametersMng;
  PropertyManager m_propertiesMng;
  Mode m_propertyViewMode;
  std::unique_ptr<RengaPropertyController> m_pPropertyController;
};
