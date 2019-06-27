#include "stdafx.h"

#include "ReinforcementUnitUsagePropertyViewBuilder.h"

#include <Renga/QuantityIds.h>

ReinforcementUnitUsagePropertyViewBuilder::ReinforcementUnitUsagePropertyViewBuilder(
  const PropertyManagers* pPropertyManagers,
  Renga::IApplicationPtr pApplication,
  Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage) :
  m_pPropertyManagers(pPropertyManagers),
  m_pApplication(pApplication),
  m_pReinforcementUnitUsage(pReinforcementUnitUsage)
{
}

void ReinforcementUnitUsagePropertyViewBuilder::createParametersProperties(PropertyList& propertyList)
{
  auto pReinforcementUnitStyle = getReinforcementUnitStyle(m_pReinforcementUnitUsage->StyleId);

  if (pReinforcementUnitStyle != nullptr)
    m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pReinforcementUnitStyle->Name));
}

void ReinforcementUnitUsagePropertyViewBuilder::createParametersPropertiesEx(PropertyList& propertyList)
{
}

void ReinforcementUnitUsagePropertyViewBuilder::createQuantitiesProperties(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pReinforcementUnitUsage->GetQuantities();

  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities, QuantityIds::TotalRebarLength);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities, QuantityIds::TotalRebarMass);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "reinforcementUnitCount"), pQuantities, QuantityIds::ReinforcementUnitCount);
}

PropertyList ReinforcementUnitUsagePropertyViewBuilder::createUserAttributesProperties()
{
  return PropertyList();
}

Renga::IReinforcementUnitStylePtr ReinforcementUnitUsagePropertyViewBuilder::getReinforcementUnitStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetUnitStyle(styleId);
}
