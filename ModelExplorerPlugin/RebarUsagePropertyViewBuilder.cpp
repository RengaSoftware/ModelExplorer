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

void RebarUsagePropertyViewBuilder::createParameters(PropertyManager& mngr, PropertyList& propertyList)
{
  auto pRebarUsage = m_rebarUsageAccess();
  if (pRebarUsage == nullptr)
    return;

  auto pRebarStyle = getRebarStyle(pRebarUsage->StyleId);

  if (pRebarStyle != nullptr)
    mngr.addValue(propertyList, QApplication::translate("me_reinforcement", "name"), QString::fromWCharArray(pRebarStyle->Name));
}

void RebarUsagePropertyViewBuilder::createQuantities(PropertyManager& mngr, PropertyList& propertyList)
{
  using namespace Renga;

  auto pRebarUsage = m_rebarUsageAccess();
  if (pRebarUsage == nullptr)
    return;

  auto pQuantities = pRebarUsage->GetQuantities();

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
