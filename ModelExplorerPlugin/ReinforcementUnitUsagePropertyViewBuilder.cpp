#include "stdafx.h"

#include "ReinforcementUnitUsagePropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/Quantities.h>


ReinforcementUnitUsagePropertyViewBuilder::ReinforcementUnitUsagePropertyViewBuilder(
    Renga::IApplicationPtr pApplication,
    ReinforcementUnitUsageAccess reinforcementUnitUsageAccess)
  : m_pApplication(pApplication),
    m_reinforcementUnitUsageAccess(reinforcementUnitUsageAccess)
{
}

void ReinforcementUnitUsagePropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  auto pReinforcementUnitUsage = m_reinforcementUnitUsageAccess();
  if (pReinforcementUnitUsage == nullptr)
    return;

  auto pReinforcementUnitStyle = getReinforcementUnitStyle(pReinforcementUnitUsage->StyleId);
  if (pReinforcementUnitStyle != nullptr)
    mngr.addValue(QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pReinforcementUnitStyle->Name));
}

void ReinforcementUnitUsagePropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  using namespace Renga;

  auto pReinforcementUnitUsage = m_reinforcementUnitUsageAccess();
  if (pReinforcementUnitUsage == nullptr)
    return;

  auto pQuantities = pReinforcementUnitUsage->GetQuantities();
  mngr.addValue(
      QApplication::translate("me_reinforcement", "totalRebarLength"),
      pQuantities->Get(Quantities::TotalRebarLength));
  mngr.addValue(
      QApplication::translate("me_reinforcement", "totalRebarMass"),
      pQuantities->Get(Quantities::TotalRebarMass));
  mngr.addValue(
      QApplication::translate("me_reinforcement", "reinforcementUnitCount"),
      pQuantities->Get(Quantities::ReinforcementUnitCount));
}

Renga::IReinforcementUnitStylePtr ReinforcementUnitUsagePropertyViewBuilder::getReinforcementUnitStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetUnitStyle(styleId);
}
