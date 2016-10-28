//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "PropertyManagers.h"
#include "IObjectPropertyViewBuilder.h"

#include <qttreepropertybrowser.h>

#include <RengaAPI/ObjectId.h>
#include <RengaAPI/UserAttributeValue.h>

class ObjectPropertyView : public QtTreePropertyBrowser
{
  Q_OBJECT

public:
	enum Mode
	{
		CategoryMode = 0,
		ListMode = 1,
	};
	
	ObjectPropertyView(QTranslator* pTranslator, QWidget* pParent);
	void setSelectedObjectId(rengaapi::ObjectId objId);
	void changeMode(ObjectPropertyView::Mode newMode);

private slots:
  void userDoubleAttributeChanged(QtProperty* property, const QString& newValue);
  void userStringAttributeChanged(QtProperty* property, const QString& newValue);

private:
  void initPropertyManagers();
  void clearPropertyManagers();
  bool createProperties(PropertyMap& parameters, PropertyMap& calculated, PropertyMap& properties);
  void buildPropertyViewAsList(const PropertyMap& parameters, const PropertyMap& calculated, const PropertyMap& properties);
  void buildPropertyViewByCategory(const PropertyMap& parameters, const PropertyMap& calculated, const PropertyMap& properties);
  void buildPropertyViewSingleCategory(const QString& categoryName, const PropertyMap& categoryProperties);
  void buildPropertyView();
  void changeUserAttribute(QtProperty* property, rengaapi::UserAttributeValue userAttributeValue);

private:
  QTranslator* m_pTranslator;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManagers m_propertyManagers;
	rengaapi::ObjectId m_currentObjectId;
	Mode m_propertyViewMode;
};