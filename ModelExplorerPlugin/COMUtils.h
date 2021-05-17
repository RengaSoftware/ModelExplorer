#pragma once

#include <string>


GUID GuidFromString(const std::string& str);
std::string GuidToString(const GUID guid);
QStringList safeArrayToQStringList(SAFEARRAY* pArray);