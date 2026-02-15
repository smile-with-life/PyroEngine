#pragma once

#include "CoreType.h"

// ==================== 编译器检测 ====================
#if defined(_MSC_VER) && !defined(__clang__) 
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "Microsoft Visual C++"
    #define COMPILER_MSVC_VERSION _MSC_VER
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
    #define COMPILER_INTEL 1
    #define COMPILER_NAME "Intel C++"
    #define COMPILER_INTEL_VERSION __INTEL_COMPILER
#elif defined(__clang__)
    #define COMPILER_CLANG 1
    #define COMPILER_NAME "Clang"
    #define COMPILER_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__GNUC__)
    #define COMPILER_GCC 1
    #define COMPILER_NAME "GCC"
    #define COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
    #error The compiler is not supported!
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
    #error The CPU architecture is not supported!
#endif

// ==================== 数据位宽检测 ====================
#if defined(__x86_64) || defined(__amd64__) || defined(__aarch64__) || \
    defined(_M_AMD64) || defined(_M_X64) || defined(_M_ARM64)    
    #define DATA_BIT_64
#else
    #define DATA_BIT_32
#endif

#if !defined(DATA_BIT_64)
    #error "The engine requires a 64-bit CPU architecture"
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
    #define DLLEXPORT
    #define DLLIMPORT
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
    #error The platform is not supported!
#endif

// ==================== 编译器扩展支持 ====================​​
#ifdef COMPILER_MSVC
    // 检查编译器版本
    #if _MSC_VER < 1930
        #error "MSVC version too low (" STRINGIZE(_MSC_VER) "). Please upgrade to VS 2022 17.0+ (_MSC_VER >= 1930) for full C++20 support."
    #endif
    // 编译器假设宏
    #define ASSUME(expr) __assume(expr)
#elif COMPILER_INTEL
    // 检查编译器版本
    #if __INTEL_COMPILER < 202100  // Intel oneAPI 2021.0+ 支持C++20
        #error "Intel compiler version too low (" STRINGIZE(__INTEL_COMPILER) "). Please upgrade to Intel oneAPI 2021.0+ for full C++20 support."
    #endif
    // 编译器假设宏
    #ifdef __INTEL_ASSUME
        #define ASSUME(expr) __INTEL_ASSUME(expr)
    #else
        #define ASSUME(expr) ((void)0)
    #endif
#elif COMPILER_CLANG
    // 检查编译器版本
    #if __clang_major__ < 12
        #error "Clang version too low (" STRINGIZE(__clang_major__) "." STRINGIZE(__clang_minor__) "). Please upgrade to Clang 12+ for full C++20 support."
    #endif 
    // 编译器假设宏
    #define ASSUME(expr) __builtin_assume(expr)
#elif COMPILER_GCC
    // 检查编译器版本
    #if __GNUC__ < 11
        #error "GCC version too low (" STRINGIZE(__GNUC__) "." STRINGIZE(__GNUC_MINOR__) "). Please upgrade to GCC 11+ for full C++20 support."
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
    #error The compiler is not supported!
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
