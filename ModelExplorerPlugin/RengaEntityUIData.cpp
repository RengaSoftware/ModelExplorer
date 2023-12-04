#include "stdafx.h"

#include "RengaEntityUIData.h"
#include "GuidMap.h"

#include <Renga/ObjectTypes.h>
#include <Renga/StyleTypeIds.h>

#include <map>


const RengaEntityUIData getRengaEntityUIData(GUID entityType)
{
  static GuidMap<RengaEntityUIData> dict
  {
    { Renga::ObjectTypes::Wall, { QApplication::translate("me_modelObjects", "Walls"), ":/icons/Wall" } },
    { Renga::ObjectTypes::Column, { QApplication::translate("me_modelObjects", "Columns"), ":/icons/Column" } },
    { Renga::ObjectTypes::Floor, { QApplication::translate("me_modelObjects", "Floors"), ":/icons/Floor" } },
    { Renga::ObjectTypes::Opening, { QApplication::translate("me_modelObjects", "Openings"), ":/icons/Opening" } },
    { Renga::ObjectTypes::Roof, { QApplication::translate("me_modelObjects", "Roofs"), ":/icons/Roof" } },
    { Renga::ObjectTypes::Beam, { QApplication::translate("me_modelObjects", "Beams"), ":/icons/Beam" } },
    { Renga::ObjectTypes::Stair, { QApplication::translate("me_modelObjects", "Stairs"), ":/icons/Stair" } },
    { Renga::ObjectTypes::Ramp, { QApplication::translate("me_modelObjects", "Ramps"), ":/icons/Ramp" } },
    { Renga::ObjectTypes::Window, { QApplication::translate("me_modelObjects", "Windows"), ":/icons/Window" } },
    { Renga::ObjectTypes::Door, { QApplication::translate("me_modelObjects", "Doors"), ":/icons/Door" } },
    { Renga::ObjectTypes::Railing, { QApplication::translate("me_modelObjects", "Railings"), ":/icons/Railing" } },
    { Renga::ObjectTypes::Room, { QApplication::translate("me_modelObjects", "Rooms"), ":/icons/Room" } },
    { Renga::ObjectTypes::IsolatedFoundation, { QApplication::translate("me_modelObjects", "Isolated foundations"), ":/icons/Isolated_foundation" } },
    { Renga::ObjectTypes::WallFoundation, { QApplication::translate("me_modelObjects", "Wall foundations"), ":/icons/Wall_foundation" } },
    { Renga::ObjectTypes::AssemblyInstance, { QApplication::translate("me_modelObjects", "Assembly instances"), ":/icons/Assembly" } },
    { Renga::ObjectTypes::Element, { QApplication::translate("me_modelObjects", "Elements"), ":/icons/Element" } },
    { Renga::ObjectTypes::Plate, { QApplication::translate("me_modelObjects", "Plates"), ":/icons/Plate" } },
    { Renga::ObjectTypes::RoutePoint, { QApplication::translate("me_modelObjects", "RoutePoints"), ":/icons/RoutePoint" } },
    { Renga::ObjectTypes::Equipment, { QApplication::translate("me_modelObjects", "Equipment"), ":/icons/Equipment" } },
    { Renga::ObjectTypes::PlumbingFixture, { QApplication::translate("me_modelObjects", "PlumbingFixtures"), ":/icons/PlumbingFixture" } },
    { Renga::ObjectTypes::MechanicalEquipment, { QApplication::translate("me_modelObjects", "AirEquipment"), ":/icons/AirEquipment" } },
    { Renga::ObjectTypes::Line3D, { QApplication::translate("me_modelObjects", "Lines3D"), ":/icons/Line" } },
    { Renga::ObjectTypes::Hatch, { QApplication::translate("me_modelObjects", "Hatches"), ":/icons/Hatch" } },
    { Renga::ObjectTypes::TextObject, { QApplication::translate("me_modelObjects", "TextShapes"), ":/icons/Text" } },
    { Renga::ObjectTypes::Rebar, { QApplication::translate("me_modelObjects", "Rebars"), ":/icons/Plate" } },
    { Renga::ObjectTypes::PipeAccessory, { QApplication::translate("me_modelObjects", "PipeAccessories"), ":/icons/PipeAccessory" } },
    { Renga::ObjectTypes::PipeFitting, { QApplication::translate("me_modelObjects", "PipeFittings"), ":/icons/PipeFitting" } },
    { Renga::ObjectTypes::Pipe, { QApplication::translate("me_modelObjects", "Pipes"), ":/icons/Pipe" } },
    { Renga::ObjectTypes::Duct, { QApplication::translate("me_modelObjects", "AirDucts"), ":/icons/AirDuct" } },
    { Renga::ObjectTypes::DuctAccessory, { QApplication::translate("me_modelObjects", "AirAccessories"), ":/icons/AirAccessory" } },
    { Renga::ObjectTypes::DuctFitting, { QApplication::translate("me_modelObjects", "AirFittings"), ":/icons/AirComponent" } },
    { Renga::ObjectTypes::LineElectricalCircuit, { QApplication::translate("me_modelObjects", "LineElectricalCircuits"), ":/icons/LineElectricalCircuit" } },
    { Renga::ObjectTypes::LightFixture, { QApplication::translate("me_modelObjects", "LightFixtures"), ":/icons/LightFixture" } },
    { Renga::ObjectTypes::ElectricDistributionBoard, { QApplication::translate("me_modelObjects", "DistributionBoards"), ":/icons/DistributionBoard" } },
    { Renga::ObjectTypes::WiringAccessory, { QApplication::translate("me_modelObjects", "WiringAccessories"), ":/icons/WiringAccessory" } },
    { Renga::ObjectTypes::Route, { QApplication::translate("me_modelObjects", "Routes"), ":/icons/Route" } },
    { Renga::ObjectTypes::Axis, { QApplication::translate("me_modelObjects", "Axes"), ":/icons/Axis" } },
    { Renga::ObjectTypes::Elevation, { QApplication::translate("me_modelObjects", "Elevations"), ":/icons/Elevation" } },
    { Renga::ObjectTypes::Section, { QApplication::translate("me_modelObjects", "Sections"), ":/icons/Section" } },
    { Renga::ObjectTypes::Dimension, { QApplication::translate("me_modelObjects", "Dimensions"), ":/icons/Dimension" } },
    { Renga::StyleTypeIds::Material ,{ "", ":/icons/Material" } },
    { Renga::StyleTypeIds::LayeredMaterial ,{ "", ":/icons/Material" } },
    { Renga::StyleTypeIds::PlumbingFixtureStyle , { "", ":/icons/i16_plumbing_fixture_style" } },
    { Renga::StyleTypeIds::EquipmentStyle ,{ "", ":/icons/i16_mep_equipment_style_editor" } },
    { Renga::StyleTypeIds::PipeStyle ,{ "", ":/icons/i16_mep_pipe_style_editor" } },
    { Renga::StyleTypeIds::PipeFittingStyle ,{ "", ":/icons/i16_pipe_component_style_editor" } },
    { Renga::StyleTypeIds::PipeAccessoryStyle ,{ "", ":/icons/i16_mep_pipe_accessory_style_editor" } },
    { Renga::StyleTypeIds::MechanicalEquipmentStyle ,{ "", ":/icons/i16_air_equipment_style_editor" } },
    { Renga::StyleTypeIds::DuctStyle ,{ "", ":/icons/i16_airduct_style_editor" } },
    { Renga::StyleTypeIds::DuctFittingStyle ,{ "", ":/icons/i16_air_component_style_editor" } },
    { Renga::StyleTypeIds::DuctAccessoryStyle ,{ "", ":/icons/i16_mep_air_accessory_style_editor" } },
    { Renga::StyleTypeIds::WiringAccessoryStyle ,{ "", ":/icons/i16_wiring_accessory_style_editor" } },
    { Renga::StyleTypeIds::LightingFixtureStyle ,{ "", ":/icons/i16_light_fixture_style_editor" } },
    { Renga::StyleTypeIds::ElectricDistributionBoardStyle ,{ "", ":/icons/i16_electrical_distribution_board_style_editor" } },
    { Renga::StyleTypeIds::ElectricalConductorStyle ,{ "", ":/icons/i16_conductors_style_editor" } },
    { Renga::StyleTypeIds::ElectricCircuitLineStyle ,{ "", ":/icons/i16_line_electrical_circuit_style_editor" } },
    { Renga::StyleTypeIds::SystemStyle, { "", ":/icons/i16_mep_system_style_editor" } },
    { Renga::StyleTypeIds::Assembly,{ "", ":/icons/Assembly" } },
  };
  auto dataIt = dict.find(entityType);
  return (dataIt != dict.end()) ? dataIt->second : RengaEntityUIData();
}
