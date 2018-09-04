#include "IPropertyViewSourceObject.h"

class PropertyViewReinforcementUnitUsageSource : public IPropertyViewSourceObject
{
public:
  PropertyViewReinforcementUnitUsageSource(
    Renga::IApplicationPtr pApplication,
    Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  IPropertyViewBuilder* createPropertyViewBuilder(PropertyManagers* pPropertyManagers) override;

  Renga::IPropertyPtr getUserDefinedProperty(GUID propertyId) override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
