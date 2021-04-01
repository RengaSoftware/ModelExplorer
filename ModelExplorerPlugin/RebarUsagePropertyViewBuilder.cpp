#include "stdafx.h"

#include "RebarUsagePropertyViewBuilder.h"
#include "PropertyManager.h"

#include <Renga/QuantityIds.h>


RebarUsagePropertyViewBuilder::RebarUsagePropertyViewBuilder(Renga::IApplicationPtr pApplication,
                                                             RebarUsageAccess rebarUsageAccess) :
  m_pApplication(pApplication),
  m_rebarUsageAccess(rebarUsageAccess)
{
}

PropertyList RebarUsagePropertyViewBuilder::createParameters(PropertyManager& mngr)
{
  auto result = PropertyList();
  auto pRebarUsage = m_rebarUsageAccess();
  if (pRebarUsage == nullptr)
    return result;

  auto pRebarStyle = getRebarStyle(pRebarUsage->StyleId);

  if (pRebarStyle != nullptr)
    mngr.addValue(result, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pRebarStyle->Name));
  return result;
}

PropertyList RebarUsagePropertyViewBuilder::createQuantities(PropertyManager& mngr)
{
  using namespace Renga;

  auto result = PropertyList();
  auto pRebarUsage = m_rebarUsageAccess();
  if (pRebarUsage == nullptr)
    return result;

  auto pQuantities = pRebarUsage->GetQuantities();

  mngr.addValue(result, QApplication::translate("me_reinforcement", "count"), pQuantities->Get(QuantityIds::Count));
  mngr.addValue(result, QApplication::translate("me_reinforcement", "nominalLength"), pQuantities->Get(QuantityIds::NominalLength));
  mngr.addValue(result, QApplication::translate("me_reinforcement", "totalRebarLength"), pQuantities->Get(QuantityIds::TotalRebarLength));
  mngr.addValue(result, QApplication::translate("me_reinforcement", "totalRebarMass"), pQuantities->Get(QuantityIds::TotalRebarMass));

  return result;
}

Renga::IRebarStylePtr RebarUsagePropertyViewBuilder::getRebarStyle(int styleId) const
{
  auto pProject = m_pApplication->Project;
  if (pProject == nullptr)
    return nullptr;

  return pProject->ReinforcementUnitStyleManager->GetRebarStyle(styleId);
}
