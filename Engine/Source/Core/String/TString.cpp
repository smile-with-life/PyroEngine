#include "pch.h"

#include "TString.h"

TString::TString(const TString& other)
    : m_data(other.m_data)
{
    
}

TString& TString::operator=(const TString& other)
{
    if (this != &other)
    {
        TString temp(other);
        m_data.Swap(temp.m_data);
    }
    return *this;
}

TString::TString(TString&& other) noexcept
    : m_data(std::move(other.m_data))
{

}

TString& TString::operator=(TString&& other) noexcept
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
    }
    return *this;
}

TString::TString(const tchar* tstr)
{
    if (tstr == nullptr)
    {
        // 空指针：构造空字符串（包含一个空字符）
        m_data.Resize(1);
        m_data[0] = 0;  // 假设 0 是空字符，对于宽字符也适用
        return;
    }

    int64 length = tcslen(tstr);
    if (length == 0)
    {
        m_data.Resize(1);
        m_data[0] = 0;
        return;
    }

    m_data.Resize(length + 1);
    for (int64 i = 0; i < length; ++i)
    {
        m_data[i] = tstr[i];
    }
    m_data[length] = 0;
}

TString& TString::operator=(const tchar* tstr)
{
    TString temp(tstr);
    std::swap(m_data, temp.m_data);
    return *this;
}

const tchar* TString::Data() const
{
    return m_data.Data();
}

int64 TString::Length() const
{
    int64 size = m_data.Size();
    return (size > 0) ? size - 1 : 0;
}

bool TString::IsEmpty() const
{
    return Length() == 0;
}

TString::operator const tchar* () const
{
    return m_data.Data();
}