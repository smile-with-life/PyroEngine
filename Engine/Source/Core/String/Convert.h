#pragma once
#include "Core.h"
#include "String.h"
#include "Platform.h"

namespace Convert
{ 
std::u16string UTF8ToUTF16(const std::string& u8str);

std::string UTF16ToUTF8(const std::u16string& u16str);

const tchar* ToNativeString(const String& str);
}// namespace Convert