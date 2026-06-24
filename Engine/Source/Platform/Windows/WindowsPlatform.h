#pragma once


#define NOMINMAX // 禁用 min/max 宏定义（避免与 std::min/max 冲突）

#include <Windows.h>

#undef CreateWindow // 避免与用户定义的 CreateWindow 函数冲突
#undef CreateSemaphore // 避免与用户定义的 CreateSemaphore 函数冲突

#include "Platform.h"

// ================== 最低系统要求 ==================
#if WINVER < 0x0A00  // Windows 10
    #error "Windows 10 or newer required"
#endif

// Windows应用程序句柄
extern "C" extern CORE_API HINSTANCE GWindowsInstance;

// SEH 异常处理支持
#define PLATFORM_SEH_EXCEPTIONS 1

// COM 接口对象安全释放
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// 兼容性适配，解决 Windows SDK 头文件在不同编译器下的编译错误
#if defined(COMPILER_CLANG) || (defined(COMPILER_MSVC_VERSION) && (COMPILER_MSVC_VERSION >= 1900))
    #include <intsafe.h>
#if defined(COMPILER_MSVC_VERSION) && (COMPILER_MSVC_VERSION >= 1940)
    #define static inline
#endif
    #include <strsafe.h>
    #undef static
#endif