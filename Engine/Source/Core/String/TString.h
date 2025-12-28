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
    TString(const TString& other)
    {
        m_data = other.m_data;
    }

    // 拷贝赋值
    TString& operator=(const TString& other)
    {
        if (this != &other) // 防止自赋值
        {
            TString temp(other); // 先创建临时对象（拷贝）
            std::swap(m_data, temp.m_data); // 交换内部数据
        }
        return *this;
    }

    // 移动构造函数
    TString(TString&& other) noexcept
    {
        m_data = std::move(other.m_data);
    }

    // 移动赋值
    TString& operator=(TString&& other) noexcept
    {
        if (this != &other) // 防止自赋值
        {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    // 从 const tchar* 构造
    explicit TString(const tchar* tstr)
    {
        if (tstr == nullptr)
        {
            // 空指针构造空字符串
            m_data.Clear();
            return;
        }

        // 计算字符串长度（不含终止符）
        int64 length = Platform::TStrLength(tstr);
        if (length == 0)
        {
            m_data.Clear();
            return;
        }

        // 分配内存（+1 用于存储终止符 TCHAR_NULL）
        m_data.Resize(length + 1);
        // 拷贝字符串数据
        m_data = Array(tstr, length);
        // 确保最后一位是终止符（防御性编程）
        m_data.Data()[length] = '\0';
    }

    // 从 const tchar* 赋值
    TString& operator=(const tchar* tstr)
    {
        if (tstr == nullptr)
        {
            m_data.Clear();
            return *this;
        }

        // 创建临时对象，再交换（异常安全）
        TString temp(tstr);
        std::swap(m_data, temp.m_data);
        return *this;
    }
public:
    // 隐式转换问 bool 值
    operator bool() const
    {
        return !m_data.IsEmpty();
    }

    operator const tchar* () const
    {
        return m_data.IsEmpty() ? nullptr : m_data.Data();
    }

    const tchar* CStr() const
    {
        return m_data.IsEmpty() ? nullptr : m_data.Data();
    }
private:
    Array<tchar> m_data;
};