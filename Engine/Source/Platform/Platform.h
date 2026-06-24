#pragma once

#include "CoreType.h"

// ==================== 编译器检测 ====================
#if defined(_MSC_VER) && !defined(__clang__) 
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "Microsoft Visual C++"
    #define COMPILER_MSVC_VERSION _MSC_VER
#elif defined(__clang__)
    #define COMPILER_CLANG 1
    #define COMPILER_NAME "Clang"
    #define COMPILER_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GCC 1
    #define COMPILER_NAME "GCC"
    #define COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
    #error "使用不支持的编译器，当前仅支持 MSVC Clang GCC 三大主流编译器"
#endif

// ==================== CPU架构检测 ====================
#if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64) || defined(_M_AMD64)
    #define CPU_ARCH_X64
    #define CPU_ARCH_NAME "x86-64"
#elif defined(__i386__) || defined(_M_IX86)
    #define CPU_ARCH_X86 
    #define CPU_ARCH_NAME "x86"
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define CPU_ARCH_ARM64
    #define CPU_ARCH_NAME "ARM64"
#elif defined(__arm__) || defined(_M_ARM)
    #define CPU_ARCH_ARM
    #define CPU_ARCH_NAME "ARM"
#else
    #error "当前 CPU 架构不被支持"
#endif

// ==================== 数据位宽检测 ====================
#if defined(__x86_64) || defined(__amd64__) || defined(__aarch64__) || \
    defined(_M_AMD64) || defined(_M_X64) || defined(_M_ARM64)    
    #define DATA_BIT_64
#else
    #define DATA_BIT_32
#endif

#if !defined(DATA_BIT_64)
    #error "当前引擎仅支持 64 位架构 CPU"
#endif

// ==================== DLL 导入导出指令定义 ====================
#ifdef PLATFORM_WINDOWS
    #define DLLEXPORT __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)
    #define PLATFORM_NAME "Windows"
#elif PLATFORM_LINUX
    #define DLLEXPORT __attribute__((visibility("default")))
    #define DLLIMPORT __attribute__((visibility("default")))
    #define PLATFORM_NAME "Linux"
#elif PLATFORM_ANDROID
    #define DLLEXPORT __attribute__((visibility("default")))
    #define DLLIMPORT __attribute__((visibility("default")))
    #define PLATFORM_NAME "Android"
#elif PLATFORM_IOS
    #define DLLEXPORT __attribute__((visibility("default")))
    #define DLLIMPORT __attribute__((visibility("default")))
    #define PLATFORM_NAME "IOS"
    #include <stddef.h>
#elif PLATFORM_MAC
    #define DLLEXPORT __attribute__((visibility("default")))
    #define DLLIMPORT __attribute__((visibility("default")))
    #define PLATFORM_NAME "Mac"
    #include <stddef.h>
#else
    #error "当前平台不被支持，目前仅支持 Windows Linux Android IOS Mac 平台"
#endif

// ==================== 符号导出控制 ====================​​
#ifdef BUILD_EXPORT_DLL
    #define CORE_API DLLEXPORT
#else
    #define CORE_API DLLIMPORT
#endif

// ==================== 字节序检测 ====================
// @ 实现

// ==================== 平台扩展支持 ====================​​
#ifdef PLATFORM_WINDOWS
    #include "Windows/WindowsPlatform.h"
#elif PLATFORM_LINUX

#elif PLATFORM_ANDROID
    
#elif PLATFORM_IOS
    
#elif PLATFORM_MAC
    
#else
    #error "当前平台不被支持，目前仅支持 Windows Linux Android IOS Mac 平台"
#endif

// ==================== 编译器扩展支持 ====================​​
#ifdef COMPILER_MSVC
    // 检查编译器版本
    #if _MSC_VER < 1930
        #error "MSVC 版本过低，请升级到 VS 2022 17.0 以获得完整的 C++20 支持"
    #endif
    // 编译器假设宏
    #define ASSUME(expr) __assume(expr)
#elif COMPILER_CLANG
    // 检查编译器版本
    #if __clang_major__ < 12
        #error "Clang 版本过低，请升级到 Clang 12+ 以获得完整的 C++20 支持"
    #endif 
    // 编译器假设宏
    #define ASSUME(expr) __builtin_assume(expr)
#elif COMPILER_GCC
    // 检查编译器版本
    #if __GNUC__ < 11
        #error "GCC 版本过低，请升级到 GCC 11+ 以获得完整的 C++20 支持"
    #endif
    // 编译器假设宏
    #ifdef __has_builtin
        #if __has_builtin(__builtin_unreachable)
            #define ASSUME(expr) do { if (!(expr)) __builtin_unreachable(); } while(0)
        #else
            #define ASSUME(expr) ((void)0)
        #endif
    #else
        #define ASSUME(expr) ((void)0)
    #endif
#else
    #error "使用不支持的编译器，当前仅支持 MSVC Clang GCC 三大主流编译器"
#endif

// ==================== 构建配置检测 ====================​​
#ifdef BUILD_CONFIG_DEBUG
    #define BUILD_CONFIG BUILD_CONFIG_DEBUG
#elif BUILD_CONFIG_DEVELOPMENT
    #define BUILD_CONFIG BUILD_CONFIG_DEVELOPMENT   
#elif BUILD_CONFIG_TEST
    #define BUILD_CONFIG BUILD_CONFIG_TEST
#elif BUILD_CONFIG_RELEASE
    #define BUILD_CONFIG BUILD_CONFIG_RELEASE
#endif

// ==================== 跨平台字符字面量 ====================​​
#ifdef PLATFORM_WINDOWS
    #define PLATFORM_NATIVE_UTF16 1
#else
    #define PLATFORM_NATIVE_UTF8 1
#endif

#ifdef PLATFORM_NATIVE_UTF8
    using tchar = char;
    #define TEXT(x) ## x
#elif PLATFORM_NATIVE_UTF16
    using tchar = wchar_t;
    #define TEXT(x) L ## x
#elif PLATFORM_NATIVE_UTF32
    using tchar = char32_t;
    #define TEXT(x)
#endif

inline int64 tcslen(const tchar* str)
{
    if (!str)
        return 0;
#if PLATFORM_NATIVE_UTF8
    // UTF-8 字符数计算（跳过多字节）
    int64 len = 0;
    while (*str != 0)
    {
        uint8 c = static_cast<uint8>(*str);
        len++;
        // 跳过后续字节（多字节编码）
        if ((c & 0x80) != 0)
        {
            if ((c & 0xE0) == 0xC0) str += 1;
            else if ((c & 0xF0) == 0xE0) str += 2;
            else if ((c & 0xF8) == 0xF0) str += 3;
        }
        str++;
    }
    return len;
#else
    // UTF-16 字符数计算（简化版，暂不处理代理对）
    int64 len = 0;
    while (*str != 0) { len++; str++; }
    return len;
#endif
}

namespace Platform
{
/// <summary>
/// 平台异常中止函数
/// </summary>
void PlatformAbort();
}
