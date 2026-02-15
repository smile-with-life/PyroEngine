#pragma once

#include "Core.h"
#include "Platform.h"


/* ==================== 核心断言宏系统 ====================​​ */

// 断言
#ifdef BUILD_CONFIG_RELEASE
    #define Assert(expr) ((void)0)
#else
    #include<assert.h>
    #define Assert(expr) assert(expr)
#endif

/// <summary>
/// 检查宏实现
/// </summary>
#define CHECK_IMPL(expr)

#ifdef BUILD_CONFIG_RELEASE // 发布版本
    /// <summary>
    /// 用于内部一致性检查，表达式无副作用
    /// <para>在发布版本中不执行</para>
    /// </summary>
    #define check(expr)    
    /// <summary>
    /// 用于必须执行的验证，表达式始终执行
    /// </summary>
    #define verify(expr) { expr; }
#else
    /// <summary>
    /// 用于内部一致性检查，表达式无副作用
    /// <para>在发布版本中不执行</para>
    /// </summary>
    #define check(expr)  CHECK_IMPL(expr)
    /// <summary>
    /// 用于必须执行的验证，表达式始终执行
    /// </summary>
    #define verify(expr) CHECK_IMPL(expr)  // 两者相同
#endif

/// <summary>
/// 代码路径不应该被执行
/// </summary>
#define codeNoEntry() ASSUME(false)

/// <summary>
/// 代码不应该被重复执行
/// </summary>
#define codeNoReentry() \
    do { \
        static bool macro_beenHere_##__LINE__ = false; \
        if (macro_beenHere_##__LINE__) { \
            ASSUME(false); /* 不应该第二次进入 */ \
        } \
        else { \
            macro_beenHere_##__LINE__ = true; \
        } \
    } while(0)

/// <summary>
/// 代码不应该在此处递归
/// </summary>
#define codeNoRecursion() \
    do { \
        static thread_local int macro_recursionDepth##__LINE__ = 0; \
        if (macro_recursionDepth##__LINE__++ > 0) { \
            ASSUME(false); \
        } \
        struct RecursionGuard \
        { \
            ~RecursionGuard() { \
                --macro_recursionDepth##__LINE__; \
            } \
        } guard; \
    } while (0)

#ifdef BUILD_CONFIG_RELEASE // 发布版本
    /// <summary>
    /// 未实现函数
    /// </summary>
    #define unimplemented() ASSUME(false)
#else
    /// <summary>
    /// 未实现函数
    /// </summary>
    #define unimplemented() \
        do { \
            Assert(false && "Function not implemented"); \
            std::terminate(); \
        } while(0)
#endif