#pragma once

#include <comdef.h>

#include <map>


struct GuidComparator
{
  bool operator () (const GUID& left, const GUID& right) const
  {
    return memcmp(&left, &right, sizeof(GUID)) < 0;
  }
};

template <typename T>
using GuidMap = std::map<GUID, T, GuidComparator>;