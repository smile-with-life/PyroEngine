#pragma once

#define NOMINMAX // 禁用 min/max 宏定义（避免与 std::min/max 冲突）

#include <Windows.h>
#include "Platform/Platform.h"

// ================== 最低系统要求 ==================
#if WINVER < 0x0A00  // Windows 10
    #error "Windows 10 or newer required"
#endif

// Windows应用程序句柄
extern "C" CORE_API HINSTANCE GWindowsInstance;