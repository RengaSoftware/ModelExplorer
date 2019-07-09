//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "IPropertyViewBuilder.h"
#include "PropertyManagers.h"

class PropertyViewBuilder : public IPropertyViewBuilder
{
public:
  PropertyViewBuilder(PropertyManagers* pPropertyManagers,
                      Renga::IApplicationPtr pApplication,
                      Renga::IModelObjectPtr pModelObject);

  // IPropertyViewBuilder
  void createIntegratedParameters(PropertyList& propertyList) override;
  void createParameters(PropertyList& propertyList) override;
  void createQuantities(PropertyList& propertyList) override;
  PropertyList createProperties() override;

  QString getMaterialName(const int& materialId);
  QString getLayeredMaterialName(const int& layeredMaterialId);

protected:
  PropertyManagers* m_pPropertyManagers;

  Renga::IApplicationPtr m_pApplication;
  Renga::IModelObjectPtr m_pModelObject;
};
