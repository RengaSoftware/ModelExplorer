#include "stdafx.h"

#include "RebarUsagePropertyViewBuilder.h"

#include <Renga/QuantityIds.h>

RebarUsagePropertyViewBuilder::RebarUsagePropertyViewBuilder(
  const PropertyManagers* pPropertyManagers,
  Renga::IApplicationPtr pApplication,
  Renga::IRebarUsagePtr pRebarUsage) :
  m_pPropertyManagers(pPropertyManagers),
  m_pApplication(pApplication),
  m_pRebarUsage(pRebarUsage)
{
}

void RebarUsagePropertyViewBuilder::createParametersProperties(PropertyList& propertyList)
{
  auto pRebarStyle = getRebarStyle(m_pRebarUsage->StyleId);

  if (pRebarStyle != nullptr)
    m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pRebarStyle->Name));
}

void RebarUsagePropertyViewBuilder::createQuantitiesProperties(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pRebarUsage->GetQuantities();

  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "count"), pQuantities, QuantityIds::Count);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "nominalLength"), pQuantities, QuantityIds::NominalLength);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities, QuantityIds::TotalRebarLength);
  m_pPropertyManagers->addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities, QuantityIds::TotalRebarMass);
}

PropertyList RebarUsagePropertyViewBuilder::createUserAttributesProperties()
{
  return PropertyList();
}

Renga::IRebarStylePtr RebarUsagePropertyViewBuilder::getRebarStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetRebarStyle(styleId);
}
