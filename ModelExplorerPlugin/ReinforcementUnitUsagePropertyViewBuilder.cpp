#include "stdafx.h"

#include "ReinforcementUnitUsagePropertyViewBuilder.h"

#include <Renga/QuantityIds.h>

ReinforcementUnitUsagePropertyViewBuilder::ReinforcementUnitUsagePropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                                                                     Renga::IApplicationPtr pApplication,
                                                                                     Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage) :
  m_pPropertyManagers(pPropertyManagers),
  m_pApplication(pApplication),
  m_pReinforcementUnitUsage(pReinforcementUnitUsage)
{
}

void ReinforcementUnitUsagePropertyViewBuilder::createIntegratedParameters(PropertyList& propertyList)
{
  auto pReinforcementUnitStyle = getReinforcementUnitStyle(m_pReinforcementUnitUsage->StyleId);

  auto& mngr = m_pPropertyManagers->m_default;
  if (pReinforcementUnitStyle != nullptr)
    mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pReinforcementUnitStyle->Name));
}

void ReinforcementUnitUsagePropertyViewBuilder::createQuantities(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pReinforcementUnitUsage->GetQuantities();

  auto& mngr = m_pPropertyManagers->m_default;
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities->Get(QuantityIds::TotalRebarLength));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities->Get(QuantityIds::TotalRebarMass));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "reinforcementUnitCount"), pQuantities->Get(QuantityIds::ReinforcementUnitCount));
}

Renga::IReinforcementUnitStylePtr ReinforcementUnitUsagePropertyViewBuilder::getReinforcementUnitStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetUnitStyle(styleId);
}
