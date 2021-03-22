#include "stdafx.h"

#include "ReinforcementUnitUsagePropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


ReinforcementUnitUsagePropertyViewBuilder::ReinforcementUnitUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                                                                     Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage) :
  m_pApplication(pApplication),
  m_pReinforcementUnitUsage(pReinforcementUnitUsage)
{
}

void ReinforcementUnitUsagePropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pReinforcementUnitStyle = getReinforcementUnitStyle(m_pReinforcementUnitUsage->StyleId);

  if (pReinforcementUnitStyle != nullptr)
    mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pReinforcementUnitStyle->Name));
}

void ReinforcementUnitUsagePropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pReinforcementUnitUsage->GetQuantities();

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
