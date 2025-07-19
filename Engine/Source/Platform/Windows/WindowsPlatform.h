#pragma once

#include <Windows.h>
#include "../Platform.h"

// ================== 最低系统要求 ==================
#if WINVER < 0x0A00  // Windows 10
    #error "Windows 10 or newer required"
#endif

// Windows应用程序句柄
extern "C" CORE_API HINSTANCE GWindowsInstance;