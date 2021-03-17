#include "IPropertyViewSourceObject.h"

class PropertyViewReinforcementUnitUsageSource : public IPropertyViewSourceObject
{
public:
  PropertyViewReinforcementUnitUsageSource(Renga::IApplicationPtr pApplication,
                                           Renga::IReinforcementUnitUsagePtr pReinforcementUnitUsage);

  std::unique_ptr<IPropertyViewBuilder> createPropertyViewBuilder() override;

private:
  Renga::IApplicationPtr m_pApplication;
  Renga::IReinforcementUnitUsagePtr m_pReinforcementUnitUsage;
};
