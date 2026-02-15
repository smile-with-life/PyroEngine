#pragma once

#include "Core.h"
#include "Platform.h"
#include "Container/Array.h"

/// <summary>
/// 跨平台本地字符串
/// </summary>
class TString
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    TString() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~TString() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    TString(const TString& other);
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    TString& operator=(const TString& other);
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    TString(TString&& other) noexcept;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    TString& operator=(TString&& other) noexcept;
    /// <summary>
    /// 构造函数，从 const tchar* 构造
    /// </summary>
    /// <param name="tstr"></param>
    explicit TString(const tchar* tstr);
    /// <summary>
    /// 从 const tchar* 赋值构造
    /// </summary>
    /// <param name="tstr"></param>
    /// <returns></returns>
    TString& operator=(const tchar* tstr);
public:
    /// <summary>
    /// 显式获取 C 风格字符串
    /// </summary>
    /// <returns></returns>
    const tchar* Data() const;
    /// <summary>
    /// 符串长度（字符数，不含终止符）
    /// </summary>
    /// <returns></returns>
    int64 Length() const;
    /// <summary>
    /// 判断是否为空字符串
    /// </summary>
    /// <returns></returns>
    bool IsEmpty() const;
public:
    /// <summary>
    /// 隐式转换问 bool 值
    /// </summary>
    operator bool() const;
    /// <summary>
    /// 隐式转换问 const tchar* 值
    /// </summary>
    operator const tchar* () const;
private:
    Array<tchar> m_data;
};