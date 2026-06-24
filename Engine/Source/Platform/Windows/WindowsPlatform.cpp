#include "pch.h"

#include "WindowsPlatform.h"

extern "C" CORE_API HINSTANCE GWindowsInstance = nullptr;

namespace Platform
{
void PlatformAbort()
{
    ::RaiseException(0xE0000001, 0, 0, nullptr);
}
}// namespace Platform
