//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyViewBuilderFactory.h"

#include "LevelPropertyViewBuilder.h"
#include "WallPropertyViewBuilder.h"
#include "ColumnPropertyViewBuilder.h"
#include "FloorPropertyViewBuilder.h"
#include "OpeningPropertyViewBuilder.h"
#include "RoofPropertyViewBuilder.h"
#include "BeamPropertyViewBuilder.h"
#include "StairPropertyViewBuilder.h"
#include "RampPropertyViewBuilder.h"
#include "WindowPropertyViewBuilder.h"
#include "DoorPropertyViewBuilder.h"
#include "RailingPropertyViewBuilder.h"
#include "RoomPropertyViewBuilder.h"
#include "IsolatedFoundationPropertyViewBuilder.h"
#include "WallFoundationPropertyViewBuilder.h"

#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>


ObjectPropertyViewBuilderFactory::ObjectPropertyViewBuilderFactory(const PropertyManagers* pPropertyManagers)
  : m_pPropertyManagers(pPropertyManagers)
{}

ObjectPropertyViewBuilder* ObjectPropertyViewBuilderFactory::createBuilder(rengaapi::ModelObject* pSelectedObject)
{
	ObjectPropertyViewBuilder* pResult = nullptr;
	
	if (pSelectedObject != nullptr)
	{
		if (pSelectedObject->type() == rengaapi::ModelObjectTypes::LevelType)
		  pResult = new LevelPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::WallType) 
			pResult = new WallPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::ColumnType) 
			pResult = new ColumnPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::FloorType)
			pResult = new FloorPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::OpeningType)
			pResult = new OpeningPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::RoofType)
			pResult = new RoofPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::BeamType)
			pResult = new BeamPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::StairType)
			pResult = new StairPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::RampType)
			pResult = new RampPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::WindowType)
			pResult = new WindowPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::DoorType)
			pResult = new DoorPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::RailingType)
			pResult = new RailingPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::RoomType)
			pResult = new RoomPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::IsolatedFoundationType)
			pResult = new IsolatedFoundationPropertyViewBuilder(m_pPropertyManagers);
		else if (pSelectedObject->type() == rengaapi::ModelObjectTypes::WallFoundationType)
			pResult = new WallFoundationPropertyViewBuilder(m_pPropertyManagers);
	}
	return pResult;
}