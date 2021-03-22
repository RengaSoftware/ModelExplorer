#include "stdafx.h"

#include "RengaModelUtils.h"

#include <Renga/StyleTypeIds.h>


// TODO: replace with universal IProject method
Renga::IEntityCollectionPtr getProjectEntityCollection(Renga::IProjectPtr pProject, GUID entityType)
{
  using namespace Renga;
  if (entityType == StyleTypeIds::PlumbingFixtureStyle)
    return pProject->PlumbingFixtureStyles;
  else if (entityType == StyleTypeIds::EquipmentStyle)
    return pProject->EquipmentStyles;
  else if (entityType == StyleTypeIds::PipeStyle)
    return pProject->PipeStyles;
  else if (entityType == StyleTypeIds::PipeAccessoryStyle)
    return pProject->PipeAccessoryStyles;
  else if (entityType == StyleTypeIds::PipeFittingStyle)
    return pProject->PipeFittingStyles;
  else if (entityType == StyleTypeIds::MechanicalEquipmentStyle)
    return pProject->MechanicalEquipmentStyles;
  else if (entityType == StyleTypeIds::DuctStyle)
    return pProject->DuctStyles;
  else if (entityType == StyleTypeIds::DuctFittingStyle)
    return pProject->DuctFittingStyles;
  else if (entityType == StyleTypeIds::DuctAccessoryStyle)
    return pProject->DuctAccessoryStyles;
  else if (entityType == StyleTypeIds::WiringAccessoryStyle)
    return pProject->WiringAccessoryStyles;
  else if (entityType == StyleTypeIds::LightFixtureStyle)
    return pProject->LightFixtureStyles;
  else if (entityType == StyleTypeIds::ElectricDistributionBoardStyle)
    return pProject->ElectricDistributionBoardStyles;
  else if (entityType == StyleTypeIds::ElectricalConductorStyle)
    return pProject->ElectricalConductorStyles;
  else if (entityType == StyleTypeIds::ElectricCircuitLineStyle)
    return pProject->ElectricalCircuitLineStyles;
  else if (entityType == StyleTypeIds::SystemStyle)
    return pProject->SystemStyles;
}