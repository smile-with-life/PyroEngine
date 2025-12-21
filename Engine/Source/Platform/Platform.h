#pragma once

#include "CoreType.h"

// ==================== 编译器检测 ====================
#if defined(_MSC_VER)
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "Microsoft Visual C++"
    #define COMPILER_MSVC_VERSION _MSC_VER
#elif defined(__INTEL_COMPILER)
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

// ==================== DLL导入导出指令定义 ====================
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

// ==================== 编译器扩展支持 ====================​​
#ifdef COMPILER_MSVC

#elif COMPILER_INTEL

#elif COMPILER_CLANG

#elif COMPILER_GCC

#else
    #error The compiler is not supported!
#endif

// ==================== 构建配置检测 ====================​​
#ifdef BUILD_CONFIG_TEST
    //#define BUILD_CONFIG_TEST 0
    #define BUILD_CONFIG BUILD_CONFIG_TEST
#elif BUILD_CONFIG_DEBUG
    //#define BUILD_CONFIG_DEBUG 1
    #define BUILD_CONFIG BUILD_CONFIG_DEBUG
#elif BUILD_CONFIG_RELEASE
    //#define BUILD_CONFIG_RELEASE 2
    #define BUILD_CONFIG BUILD_CONFIG_RELEASE
#elif BUILD_CONFIG_DIST
    //#define BUILD_CONFIG_DIST 3
    #define BUILD_CONFIG BUILD_CONFIG_DIST
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

namespace Platform
{
inline int64 StrLength(const tchar* str)
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
}
 
// ==================== 平台支持特性 ====================​​
namespace PlatformProperties
{
/// <summary>
/// 获取平台唯一标识名
/// </summary>
/// <returns>平台名称</returns>
static const char* PlatformName();

/// <summary>
/// 获取平台使用的物理引擎格式
/// </summary>
/// <returns>物理引擎名称</returns>
static const char* GetPhysicsFormat();

/// <summary>
/// 判断平台是否为小端序（字节序）
/// </summary>
/// <returns>bool</returns>
static bool IsLittleEndian();

/// <summary>
/// 判断平台是否支持窗口化运行模式
/// </summary>
/// <returns>bool</returns>
static bool IsSupportsWindowed();

/// <summary>
/// 判断平台是否支持音频流送
/// <para>大音频文件的分段加载（避免一次性加载占用内存）</para>
/// </summary>
/// <returns></returns>
static bool IsSupportsAudioStreaming();

/// <summary>
/// 判断平台是否支持高质量光照贴图
/// </summary>
/// <returns></returns>
static bool IsSupportsHighQualityLightmaps();

/// <summary>
/// 判断平台是否支持低质量光照贴图
/// </summary>
/// <returns></returns>
static bool IsSupportsLowQualityLightmaps();

/// <summary>
/// 判断平台是否支持距离场阴影
/// <para>用于开放世界场景的大面积阴影渲染优化</para>
/// </summary>
/// <returns></returns>
static bool IsSupportsDistanceFieldShadows();

/// <summary>
/// 判断平台是否支持距离场环境光遮蔽
/// </summary>
/// <returns></returns>
static bool IsSupportsDistanceFieldAO();

/// <summary>
/// 判断平台是否支持纹理流送
/// </summary>
/// <returns></returns>
static bool IsSupportsTextureStreaming();

/// <summary>
/// 判断平台是否支持虚拟纹理流送
/// </summary>
/// <returns></returns>
static bool IsSupportsVirtualTextureStreaming();

/// <summary>
/// 判断平台是否支持网格LOD流送
/// </summary>
/// <returns></returns>
static bool IsSupportsMeshLODStreaming();

/// <summary>
/// 判断平台是否支持内存映射文件（MMF）
/// <para>大文件的快速加载（避免全量读入内存）</para>
/// </summary>
/// <returns></returns>
static bool IsSupportsMemoryMappedFiles();

/// <summary>
/// 判断平台是否支持音频文件的内存映射加载
/// </summary>
/// <returns></returns>
static bool IsSupportsMemoryMappedAudio();

/// <summary>
/// 判断平台是否支持动画文件的内存映射加载
/// </summary>
/// <returns></returns>
static bool IsSupportsMemoryMappedAnimation();

/// <summary>
/// 获取内存映射文件的对齐要求（单位：字节）
/// <para>内存映射时的地址对齐（硬件强制要求，否则会崩溃）</para>
/// </summary>
/// <returns>对齐值</returns>
static int64 GetMemoryMappingAlignment();

/// <summary>
/// 判断平台是否支持硬件光线追踪
/// </summary>
/// <returns></returns>
static bool IsSupportsRayTracing();

/// <summary>
/// 判断平台是否支持Lumen全局光照
/// </summary>
/// <returns></returns>
static bool IsSupportsLumenGI();

/// <summary>
/// 判断平台是否支持硬件加速LZ解压
/// </summary>
/// <returns></returns>
static bool IsSupportsHardwareLZDecompression();

/// <summary>
/// 判断平台是否为固定分辨率
/// </summary>
/// <returns></returns>
static bool IsFixedResolution();
}
