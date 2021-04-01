#include "stdafx.h"

#include "ReinforcementUnitUsagePropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


ReinforcementUnitUsagePropertyViewBuilder::ReinforcementUnitUsagePropertyViewBuilder(
    Renga::IApplicationPtr pApplication,
    ReinforcementUnitUsageAccess reinforcementUnitUsageAccess)
  : m_pApplication(pApplication),
    m_reinforcementUnitUsageAccess(reinforcementUnitUsageAccess)
{
}

PropertyList ReinforcementUnitUsagePropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  auto result = PropertyList();

  auto pReinforcementUnitUsage = m_reinforcementUnitUsageAccess();
  if (pReinforcementUnitUsage == nullptr)
    return result;

  auto pReinforcementUnitStyle = getReinforcementUnitStyle(pReinforcementUnitUsage->StyleId);

  if (pReinforcementUnitStyle != nullptr)
    mngr.addValue(result, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pReinforcementUnitStyle->Name));
  return result;
}

PropertyList ReinforcementUnitUsagePropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  using namespace Renga;

  auto result = PropertyList();

  auto pReinforcementUnitUsage = m_reinforcementUnitUsageAccess();
  if (pReinforcementUnitUsage == nullptr)
    return result;

  auto pQuantities = pReinforcementUnitUsage->GetQuantities();
  mngr.addValue(
      result,
      QApplication::translate("me_reinforcement", "totalRebarLength"),
      pQuantities->Get(QuantityIds::TotalRebarLength));
  mngr.addValue(
      result,
      QApplication::translate("me_reinforcement", "totalRebarMass"),
      pQuantities->Get(QuantityIds::TotalRebarMass));
  mngr.addValue(
      result,
      QApplication::translate("me_reinforcement", "reinforcementUnitCount"),
      pQuantities->Get(QuantityIds::ReinforcementUnitCount));
  return result;
}

Renga::IReinforcementUnitStylePtr ReinforcementUnitUsagePropertyViewBuilder::getReinforcementUnitStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetUnitStyle(styleId);
}
