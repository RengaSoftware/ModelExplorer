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

const uint c_defaultPrecision = 2;
const uint c_userAttrPrecision = 15;

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
  bool createProperties(PropertyList& parameters, PropertyList& calculated, PropertyList& properties);
  void buildPropertyViewAsList(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& properties);
  void buildPropertyViewByCategory(const PropertyList& parameters, const PropertyList& calculated, const PropertyList& properties);
  void buildPropertyViewSingleCategory(const QString& categoryName, const PropertyList& categoryProperties);
  void buildPropertyView();
  void changeUserAttribute(QtProperty* property, rengaapi::UserAttributeValue userAttributeValue);

private:
  QTranslator* m_pTranslator;
  QtGroupPropertyManager* m_pGroupManager;
  PropertyManagers m_propertyManagers;
	rengaapi::ObjectId m_currentObjectId;
	Mode m_propertyViewMode;
};