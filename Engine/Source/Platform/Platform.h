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
    using TCHAR = char;
    #define TEXT(x) ## x
#elif PLATFORM_NATIVE_UTF16
    using TCHAR = wchar_t;
    #define TEXT(x) L ## x
#elif PLATFORM_NATIVE_UTF32
    using TCHAR = char32_t;
    #define TEXT(x)
#endif

namespace Platform
{
inline int64 TCHARLength(const TCHAR* str)
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

/// <summary>
/// 跨平台编码兼容字符串
/// <para>屏蔽不同平台字符编码差异</para>
/// </summary>
class NativeString
{
public:
    // 默认构造函数
    NativeString() = default;

    // 析构函数
    ~NativeString()
    {
        _FreeBuffer(m_data);
    }

    // 拷贝构造函数
    NativeString(const NativeString& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = _AllocBuffer(m_capacity);
        if (m_data && other.m_data)
        {
            std::memcpy(m_data, other.m_data, m_capacity * sizeof(TCHAR));
        }
    }

    // 拷贝赋值
    NativeString& operator=(const NativeString& other)
    {
        if (this == &other) 
            return *this;
        // 释放旧内存
        _FreeBuffer(m_data);

        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = _AllocBuffer(m_capacity);
        if (m_data && other.m_data)
        {
            std::memcpy(m_data, other.m_data, m_capacity * sizeof(TCHAR));
        }

        return *this;
    }

    // 移动构造函数
    NativeString(NativeString&& other) noexcept
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // 移动赋值
    NativeString& operator=(NativeString&& other) noexcept
    {
        if (this == &other) 
            return *this;

        // 释放旧内存
        _FreeBuffer(m_data);

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    // 从 TCHAR 字面量构造
    NativeString(const TCHAR* str)
    {
        if (!str) 
            return;
        m_size = TCHARLength(str);
        m_capacity = m_size + 1;  // 预留结束符空间
        m_data = _AllocBuffer(m_capacity);
        _CopyBuffer(m_data, str, m_size);
    }

    // 从 TCHAR 字面量赋值
    NativeString& operator=(const TCHAR* str)
    {
        _FreeBuffer(m_data);

        m_size = TCHARLength(str);
        m_capacity = m_size + 1;
        m_data = _AllocBuffer(m_capacity);
        _CopyBuffer(m_data, str, m_size);

        return *this;
    }

    // 隐式转换问 bool 值
    operator bool() const
    {
        return m_size;
    }
public:
    // 将字符串附加到末尾
    NativeString& Append(const NativeString& str)
    {
        // 空数组处理
        if (str.m_size == 0)
            return *this;

        int64 newSize = m_size + str.m_size;
        Reserve(newSize);

        // 拷贝拼接内容
        _CopyBuffer(m_data + m_size, str.m_data, str.m_size);
        m_size = newSize;
        // 确保结束符
        m_data[m_size] = '\0';
        return *this;
    }

    // 将字符附加到末尾
    NativeString& Append(TCHAR ch)
    {
        int64 newSize = m_size + 1;
        Reserve(newSize);



    }

    // 将字符串添加到开头
    NativeString& Prepend(const NativeString& str);

    // 将字符添加到开头
    NativeString& Prepend(TCHAR ch);

    // 比较两个字符串
    int64 Compare(const NativeString& str) const;

    // 判断字符串是否包含有给定的子串 
    bool Contains(const NativeString& str) const;

    // 判断字符串是否包含有给定的字符
    bool Contains(TCHAR ch) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int64 IndexOf(const NativeString& str) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int64 IndexOf(TCHAR ch) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int64 LastIndexOf(const NativeString& str) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int64 LastIndexOf(TCHAR ch) const;

    // 判断字符串是否有给定前缀
    bool StartWith(const NativeString& str) const;

    // 判断字符串是否有给定后缀
    bool EndWith(const NativeString& str) const;

    // 从字符串起始移除 count 个字符
    NativeString& RemoveLeft(int64 count);

    // 从字符串中间移除 count 个字符
    NativeString& RemoveMid(int64 index, int64 count);

    // 从字符串末尾移除 count 个字符
    NativeString& RemoveRight(int64 count);

    // 保留字符串最左侧的 count 个字符,其余部分舍弃掉
    NativeString& RemianLeft(int64 count);

    // 保留字符串从 index 位置开始的 count 个字符,其余部分舍弃掉
    NativeString& RemianMid(int64 index, int64 count);

    // 保留字符串最右侧的 count 个字符,其余部分舍弃掉
    NativeString& RemianRight(int64 count);

    // 返回一个字符串最左侧的 count 个字符的子串
    NativeString LeftStr(int64 count);

    // 返回一个字符串从 index 位置开始的 count 个字符的子串
    NativeString MidStr(int64 index, int64 count);

    // 返回一个字符串从 (start, end) 的子串,不包括 start 和 end
    NativeString SubStr(int64 start, int64 end);

    // 返回一个字符串最右侧的 count 个字符的子串
    NativeString RightStr(int64 count);

    // 移除字符串左右两侧的空白字符
    NativeString& Trim();

    // 移除字符串左侧的空白字符
    NativeString& TrimLeft();

    // 移除字符串右侧的空白字符
    NativeString& TrimRight();

    // 向指定位置插入字符串
    NativeString& Insert(int64 index, const NativeString& str);

    // 替换字符串的指定部分
    NativeString& Replace(int64 index, int64 count, const NativeString& str);

    // 使当前字符串重复 n 次
    NativeString& Repeat(int64 count);

    // 清空字符串
    void Clear();

    // 返回字符串预分配空间的大小
    int64 Capacity() const;

    // 返回字符数
    int64 Size() const;

    // 预分配内存大小
    void Reserve(int64 size);

    // 移除未使用的容量
    void Shrink();

    // 判断字符串是否为空
    bool IsEmpty() const;

    // 判断索引是否为有效值
    bool IsValid(int64 index) const;

    // 交换两个字符串
    void Swap(NativeString& other);

    // 将字符串转换为 C 类型的字符串
    const TCHAR* Data() const;
public:
    NativeString& operator+=(const NativeString& str)
    {
        return Append(str);
    }

    NativeString& operator+=(TCHAR ch)
    {
        return Append(ch);
    }

    friend NativeString operator+(const NativeString& left, const NativeString& right);

    friend bool operator==(const NativeString& left, const NativeString& right);

    friend bool operator==(const NativeString& left, const TCHAR* right);

    friend bool operator==(const TCHAR* left, const NativeString& right);

    friend bool operator!=(const NativeString& left, const NativeString& right);

    friend bool operator!=(const NativeString& left, const TCHAR* right);

    friend bool operator!=(const TCHAR* left, const NativeString& right);

    friend bool operator>(const NativeString& left, const NativeString& right);

    friend bool operator>(const NativeString& left, const TCHAR* right);

    friend bool operator>(const TCHAR* left, const NativeString& right);

    friend bool operator>=(const NativeString& left, const NativeString& right);

    friend bool operator>=(const NativeString& left, const TCHAR* right);

    friend bool operator>=(const TCHAR* left, const NativeString& right);

    friend bool operator<(const NativeString& left, const NativeString& right);

    friend bool operator<(const NativeString& left, const TCHAR* right);

    friend bool operator<(const TCHAR* left, const NativeString& right);

    friend bool operator<=(const NativeString& left, const NativeString& right);

    friend bool operator<=(const NativeString& left, const TCHAR* right);

    friend bool operator<=(const TCHAR* left, const NativeString& right);
private:
        // 分配缓冲区（对齐内存，避免内存碎片）
        static TCHAR* _AllocBuffer(int64 capacity)
        {
            if (capacity == 0) 
                return nullptr;
            // 用 new 分配，保证内存对齐
            return new TCHAR[capacity]();
        }

        // 释放缓冲区
        static void _FreeBuffer(TCHAR* buffer) noexcept
        {
            delete[] buffer;
        }

        // 拷贝缓冲区（处理编码差异）
        static void _CopyBuffer(TCHAR* dest, const TCHAR* src, size_t len)
        {
            if (!dest || !src || len == 0) return;
            std::memcpy(dest, src, len * sizeof(TCHAR));
            // 确保结束符
            dest[len] = '\0';
        }

        // 扩容缓冲区
        void _ResizeBuffer(size_t newCapacity)
        {
            if (newCapacity <= m_capacity) return;

            TCHAR* newBuffer = _AllocBuffer(newCapacity);
            if (newBuffer && m_data)
            {
                // 拷贝旧数据
                std::memcpy(newBuffer, m_data, m_capacity * sizeof(TCHAR));
            }

            // 释放旧缓冲区，接管新缓冲区
            _FreeBuffer(m_data);
            m_data = newBuffer;
            m_capacity = newCapacity;
        }
private:
    TCHAR* m_data = nullptr;
    int64 m_size = 0;
    int64 m_capacity = 0;
};
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
