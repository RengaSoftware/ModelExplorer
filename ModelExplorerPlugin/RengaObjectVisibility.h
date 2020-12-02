//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include <atlsafe.h>


typedef std::list<int> ObjectIdList;

static bool getRengaObjectVisibility(Renga::IApplicationPtr pApplication, const int& objectId)
{
  auto pView = pApplication->GetActiveView();

  Renga::IModelViewPtr pModelView;
  pView->QueryInterface(&pModelView);

  if (!pModelView)
    return false;

  return pModelView->IsObjectVisible(objectId);
}

static void setRengaObjectVisibility(Renga::IApplicationPtr pApplication, const ObjectIdList& objectIdList, const bool visible)
{
  auto pView = pApplication->GetActiveView();

  Renga::IModelViewPtr pModelView;
  pView->QueryInterface(&pModelView);

  if (!pModelView)
    return;

  CComSafeArray<int> idsSafeArray(static_cast<ULONG>(objectIdList.size()));
  LONG i(0);
  for (auto id : objectIdList)
    idsSafeArray.SetAt(i++, id);

  pModelView->SetObjectsVisibility(idsSafeArray, visible);
}

static void showRengaObjects(Renga::IApplicationPtr pApplication, const ObjectIdList& objectIdList)
{
  auto pView = pApplication->GetActiveView();

  Renga::IModelViewPtr pModelView;
  pView->QueryInterface(&pModelView);

  if (!pModelView)
    return;

  CComSafeArray<int> idsSafeArray(static_cast<ULONG>(objectIdList.size()));
  LONG i(0);
  for (auto id : objectIdList)
    idsSafeArray.SetAt(i++, id);

  pModelView->ShowObjects(idsSafeArray);
}
