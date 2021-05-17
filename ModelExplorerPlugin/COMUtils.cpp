#include "stdafx.h"
#include "COMUtils.h"

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

template <typename TElem, VARTYPE elemVartype, typename TFunc>
HRESULT accessSafeArrayData(SAFEARRAY* pArray, const TFunc& func)
{
  VARTYPE type;
  HRESULT hr = ::SafeArrayGetVartype(pArray, &type);
  if (FAILED(hr))
    return hr;

  if (type != elemVartype)
    return S_FALSE;

  LONG iLBound;
  hr = ::SafeArrayGetLBound(pArray, 1, &iLBound);
  if (FAILED(hr))
    return hr;

  LONG iUBound;
  hr = ::SafeArrayGetUBound(pArray, 1, &iUBound);
  if (FAILED(hr))
    return hr;

  TElem* pElemMem(nullptr);
  hr = ::SafeArrayAccessData(pArray, (void**)&pElemMem);
  if (FAILED(hr))
    return hr;

  func(pElemMem, iUBound - iLBound + 1); // TODO exception safety?

  hr = ::SafeArrayUnaccessData(pArray);
  if (FAILED(hr))
    return hr;

  return S_OK;
}

QStringList safeArrayToQStringList(SAFEARRAY* pArray)
{
  QStringList result;
  accessSafeArrayData<BSTR, VT_BSTR>(pArray, [&result](auto pData, int dataSize)
  {
    for (int i = 0; i < dataSize; ++i)
      result.push_back(QString::fromWCharArray(pData[i]));
  });
  return result;
}