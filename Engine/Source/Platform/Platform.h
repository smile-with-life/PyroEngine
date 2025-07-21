#pragma once

// ================== 编译器检测 ==================
#if defined(_MSC_VER)
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "Microsoft Visual C++"
    #define COMPILER_VERSION _MSC_VER
#elif defined(__INTEL_COMPILER)
    #define COMPILER_INTEL 1
    #define COMPILER_NAME "Intel C++"
    #define COMPILER_VERSION __INTEL_COMPILER
#elif defined(__clang__)
    #define COMPILER_CLANG 1
    #define COMPILER_NAME "Clang"
    #define COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__GNUC__)
    #define COMPILER_GCC 1
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
    #error The compiler is not supported!
#endif

// ================== CPU架构检测 ==================
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

// ================== 数据位宽检测 ==================
#if defined(__x86_64) || defined(__amd64__) || defined(__aarch64__) || \
    defined(_M_AMD64) || defined(_M_X64) || defined(_M_ARM64)    
    #define DATA_BIT_64
#else
    #define DATA_BIT_32
#endif

// ================== 强制64位检查 ==================
#if !defined(DATA_BIT_64)
    #error "The engine requires a 64-bit CPU architecture"
#endif

// ================== DLL导入导出指令定义 ==================
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

// ================== DLL导入/导出控制 ==================​​
#ifdef BUILD_EXPORT_DLL
    #define CORE_API DLLEXPORT
#else
    #define CORE_API DLLIMPORT
#endif

// ================== 字节序检测 ==================​​
// @ 实现

// ================== 平台扩展支持 ==================​​
#ifdef PLATFORM_WINDOWS
    #include "Windows/WindowsPlatform.h"
#elif PLATFORM_LINUX
    #include "Linux/LinuxPlatform.h"
#elif PLATFORM_ANDROID
    #include "Android/AndroidPlatform.h"
#elif PLATFORM_IOS
    #include "IOS/IOSPlatform.h"
#elif PLATFORM_MAC
    #include "Mac/MacPlatform.h"
#else
    #error The platform is not supported!
#endif

// ================== 平台构建定义 ==================​​
#ifdef BUILD_CONFIG_TEST
    #define BUILD_CONFIG_TEST 0
    #define BUILD_CONFIG BUILD_CONFIG_TEST
#elif BUILD_CONFIG_DEBUG
    #define BUILD_CONFIG_DEBUG 1
    #define BUILD_CONFIG BUILD_CONFIG_DEBUG
#elif BUILD_CONFIG_RELEASE
    #define BUILD_CONFIG_RELEASE 2
    #define BUILD_CONFIG BUILD_CONFIG_RELEASE
#elif BUILD_CONFIG_DIST
    #define BUILD_CONFIG_DIST 3
    #define BUILD_CONFIG BUILD_CONFIG_DIST
#endif

// ================== 平台类型定义 ==================
#include <cstdint>
#include <cstddef>
#include <stdint.h>
using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using char8 = char8_t;
using char16 = char16_t;
using char32 = char32_t;

using wchar = wchar_t;

using ptrdiff = std::ptrdiff_t;