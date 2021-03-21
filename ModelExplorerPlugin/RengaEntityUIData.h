#pragma once

struct RengaEntityUIData
{
  QString pluralName;
  QString icon16Path;
};


const RengaEntityUIData getRengaEntityUIData(GUID entityType);