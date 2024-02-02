#include "stdafx.h"

#include "RengaModelUtils.h"
#include "GuidMap.h"

#include <Renga/StyleTypeIds.h>
#include <Renga/ParameterIds.h>

#include <map>


using namespace Renga;

GUID parameterIdToEntityType(GUID parameterId)
{
  const auto c_parameterToIdDict = GuidMap<GUID>{
      {ParameterIds::MaterialStyleId, StyleTypeIds::Material},
      {ParameterIds::LayeredMaterialStyleId, StyleTypeIds::LayeredMaterial},
      {ParameterIds::PlumbingFixtureStyleId, StyleTypeIds::PlumbingFixtureStyle},
      {ParameterIds::MepEquipmentStyleId, StyleTypeIds::EquipmentStyle},
      {ParameterIds::MepPipeStyleId, StyleTypeIds::PipeStyle},
      {ParameterIds::PipeComponentStyleId, StyleTypeIds::PipeFittingStyle},
      {ParameterIds::PipeAccessoryStyleId, StyleTypeIds::PipeAccessoryStyle},
      {ParameterIds::AirEquipmentStyleId, StyleTypeIds::MechanicalEquipmentStyle},
      {ParameterIds::DuctStyleId, StyleTypeIds::DuctStyle},
      {ParameterIds::AirComponentStyleId, StyleTypeIds::DuctFittingStyle},
      {ParameterIds::AirAccessoryStyleId, StyleTypeIds::DuctAccessoryStyle},
      {ParameterIds::WiringAccessoryStyleId, StyleTypeIds::WiringAccessoryStyle},
      {ParameterIds::LightFixtureStyleId, StyleTypeIds::LightingFixtureStyle},
      {ParameterIds::DistributionBoardStyleId, StyleTypeIds::ElectricDistributionBoardStyle},
      {ParameterIds::ConductorStyleId, StyleTypeIds::ElectricalConductorStyle},
      {ParameterIds::LineElectricalCircuitStyleId, StyleTypeIds::ElectricCircuitLineStyle},
      {ParameterIds::MepSystemStyleId, StyleTypeIds::SystemStyle},
      {ParameterIds::AssemblyId, StyleTypeIds::Assembly},
      {ParameterIds::BeamStyleId, StyleTypeIds::BeamStyle},
      {ParameterIds::ColumnStyleId, StyleTypeIds::ColumnStyle},
      {ParameterIds::WindowStyleId, StyleTypeIds::WindowStyle},
      {ParameterIds::DoorStyleId, StyleTypeIds::DoorStyle},
      {ParameterIds::BuildingElementStyleId, StyleTypeIds::ElementStyle},
      {ParameterIds::PlateProfileStyleId, StyleTypeIds::PlateStyle},
      //{ParameterIds::TopicId, StyleTypeIds::Topic},
  };

  auto it = c_parameterToIdDict.find(parameterId);
  return it != c_parameterToIdDict.end() ? it->second : GUID_NULL;
}

// TODO: replace with universal IProject method
// TODO: combine with "parameter to type" dictionary
Renga::IEntityCollectionPtr getProjectEntityCollection(Renga::IProjectPtr pProject, GUID entityType)
{
  using namespace Renga::StyleTypeIds;
  using EntityCollectionGetter = std::function<IEntityCollectionPtr(IProject&)>;

  static auto entityCollectionGetterMap = GuidMap<EntityCollectionGetter>{
      {PlumbingFixtureStyle, [](IProject& project) { return project.PlumbingFixtureStyles; }},
      {EquipmentStyle, [](IProject& project) { return project.EquipmentStyles; }},
      {PipeStyle, [](IProject& project) { return project.PipeStyles; }},
      {PipeAccessoryStyle, [](IProject& project) { return project.PipeAccessoryStyles; }},
      {PipeFittingStyle, [](IProject& project) { return project.PipeFittingStyles; }},
      {MechanicalEquipmentStyle, [](IProject& project) { return project.MechanicalEquipmentStyles; }},
      {DuctStyle, [](IProject& project) { return project.DuctStyles; }},
      {DuctFittingStyle, [](IProject& project) { return project.DuctFittingStyles; }},
      {DuctAccessoryStyle, [](IProject& project) { return project.DuctAccessoryStyles; }},
      {WiringAccessoryStyle, [](IProject& project) { return project.WiringAccessoryStyles; }},
      {LightingFixtureStyle, [](IProject& project) { return project.LightFixtureStyles; }},
      {ElectricDistributionBoardStyle, [](IProject& project) { return project.ElectricDistributionBoardStyles; }},
      {ElectricalConductorStyle, [](IProject& project) { return project.ElectricalConductorStyles; }},
      {ElectricCircuitLineStyle, [](IProject& project) { return project.ElectricalCircuitLineStyles; }},
      {SystemStyle, [](IProject& project) { return project.SystemStyles; }},
      {Assembly, [](IProject& project) { return project.Assemblies; }},
      {BeamStyle, [](IProject& project) { return project.BeamStyles; }},
      {ColumnStyle, [](IProject& project) { return project.ColumnStyles; }},
      {WindowStyle, [](IProject& project) { return project.WindowStyles; }},
      {DoorStyle, [](IProject& project) { return project.DoorStyles; }},
      {ElementStyle, [](IProject& project) { return project.ElementStyles; }},
      {PlateStyle, [](IProject& project) { return project.PlateStyles; }},
      {Material, [](IProject& project) { return project.Materials; }},
      {LayeredMaterial, [](IProject& project) { return project.LayeredMaterials; }},
      //{Topic, [](IProject& project) { return project.Topics; }},
  };
 
  return entityCollectionGetterMap.at(entityType)(*pProject);
}

const QMap<Renga::Logical, QString>& logicalValueToStringMap()
{
  static auto valueList = QMap<Renga::Logical, QString>{
      {Renga::Logical::Logical_False, QApplication::translate("me_mo", "logical_false")},
      {Renga::Logical::Logical_True, QApplication::translate("me_mo", "logical_true")},
      {Renga::Logical::Logical_Indeterminate, QApplication::translate("me_mo", "logical_indeterminate")}};
  return valueList;
}

QStringList getLogicalValueStringList()
{
  QStringList result;
  for (auto valueString : logicalValueToStringMap())
    result.push_back(valueString);
  return result;
}

int getLogicalValueIndex(Renga::Logical value)
{
  auto it = logicalValueToStringMap().find(value);
  return std::distance(logicalValueToStringMap().cbegin(), it);
}

Renga::Logical getLogicalValueFromIndex(int index)
{
  auto logicalValueString = getLogicalValueStringList().at(index);
  return logicalValueToStringMap().key(logicalValueString);
}

QString getEntityName(Renga::IProjectPtr pProject, GUID entityType, int entityId)
{
  auto pCollection = getProjectEntityCollection(pProject, entityType);
  auto pEntity = pCollection->GetById(entityId);
  return pEntity ? QString::fromWCharArray(pEntity->Name) : QString{};
}

Renga::IModelObjectCollectionPtr getModelObjectsSafe(Renga::IModel& model)
{
  try
  {
    return model.GetObjects();
  }
  catch (...)
  {
    return nullptr;
  }
}
