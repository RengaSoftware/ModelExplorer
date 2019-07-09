#include "stdafx.h"

#include "RebarUsagePropertyViewBuilder.h"

#include <Renga/QuantityIds.h>

RebarUsagePropertyViewBuilder::RebarUsagePropertyViewBuilder(PropertyManagers* pPropertyManagers,
                                                             Renga::IApplicationPtr pApplication,
                                                             Renga::IRebarUsagePtr pRebarUsage) :
  m_pPropertyManagers(pPropertyManagers),
  m_pApplication(pApplication),
  m_pRebarUsage(pRebarUsage)
{
}

void RebarUsagePropertyViewBuilder::createIntegratedParameters(PropertyList& propertyList)
{
  auto pRebarStyle = getRebarStyle(m_pRebarUsage->StyleId);

  auto& mngr = m_pPropertyManagers->m_default;
  if (pRebarStyle != nullptr)
    mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pRebarStyle->Name));
}

void RebarUsagePropertyViewBuilder::createQuantities(PropertyList& propertyList)
{
  using namespace Renga;

  auto pQuantities = m_pRebarUsage->GetQuantities();

  auto& mngr = m_pPropertyManagers->m_default;
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "count"), pQuantities->Get(QuantityIds::Count));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "nominalLength"), pQuantities->Get(QuantityIds::NominalLength));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities->Get(QuantityIds::TotalRebarLength));
  mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities->Get(QuantityIds::TotalRebarMass));
}

Renga::IRebarStylePtr RebarUsagePropertyViewBuilder::getRebarStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetRebarStyle(styleId);
}
