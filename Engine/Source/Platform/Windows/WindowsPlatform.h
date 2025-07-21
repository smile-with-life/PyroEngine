#pragma once
#define WIN32_LEAN_AND_MEAN  // 排除不常用的 API（如 Shell、注册表等）
#define NOMINMAX            // 禁用 min/max 宏定义（避免与 std::min/max 冲突）
#include "Windows.h"

#include <Windows.h>
#include "../Platform.h"

// ================== 最低系统要求 ==================
#if WINVER < 0x0A00  // Windows 10
    #error "Windows 10 or newer required"
#endif

// Windows应用程序句柄
extern "C" CORE_API HINSTANCE GWindowsInstance;