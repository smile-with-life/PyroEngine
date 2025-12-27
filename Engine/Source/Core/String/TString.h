#pragma once
#include "Core.h"
#include "Container/Array.h"
#include "Platform.h"

class TString
{
public:
    // 默认构造函数
    TString() = default;

    // 析构函数
    ~TString() = default;

    // 拷贝构造函数
    TString(const TString& other);

    // 拷贝赋值
    TString& operator=(const TString& other);

    // 移动构造函数
    TString(TString&& other);

    // 移动赋值
    TString& operator=(TString&& other);

    // 从 const tchar* 构造
    TString(const tchar* tstr);

    // 从 const tchar* 赋值
    TString& operator=(const tchar* tstr);

public:
    // 隐式转换为 C 风格字符串
    operator const tchar* () const
    {
        return m_data.Data();
    }

    // 隐式转换问 bool 值
    operator bool() const
    {
        return !m_data.IsEmpty();
    }
private:
    Array<tchar> m_data;
};