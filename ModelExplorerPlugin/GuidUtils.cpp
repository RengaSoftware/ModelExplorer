#include "stdafx.h"
#include "GuidUtils.h"

#include <rpc.h>

#pragma comment(lib, "Rpcrt4.lib")


GUID GuidFromString(const std::string& str)
{
  ::GUID id;
  ::UuidFromStringA((RPC_CSTR)str.c_str(), &id);
  return id;
}

std::string GuidToString(const GUID guid)
{
  CHAR* cszUuid(nullptr);
  ::UuidToStringA(&guid, (RPC_CSTR*)&cszUuid);
  std::string ret;
  if (cszUuid)
  {
    ret = cszUuid;
    ::RpcStringFreeA((RPC_CSTR*)&cszUuid);
  }
  return ret;
}
