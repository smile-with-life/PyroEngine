#include "pch.h"

#include "Char.h"

Char::Char(const Char& other)
    : m_data(other.m_data)
{

}

Char& Char::operator=(const Char& other)
{
    m_data = other.m_data;
    return *this;
}

Char::Char(Char&& other)
    : m_data(std::move(other.m_data))
{
    
}

Char& Char::operator=(Char&& other)
{
    m_data = std::move(other.m_data);
    return *this;
}

Char::Char(char c) 
    : m_data(1, c) 
{

}

Char::Char(const std::string& utf8Char)
{
    if (!_IsValidUTF8(utf8Char))
    {
        throw std::invalid_argument("Invalid UTF-8 character");
    }
    m_data = utf8Char;
}

Char::Char(uint32 unicode)
{
    if (unicode > 0x10FFFF || (unicode >= 0xD800 && unicode <= 0xDFFF)) 
    {
        throw std::invalid_argument("Invalid Unicode code point");
    }
    m_data.clear();

    if (unicode <= 0x7F)
    { // 单字节
        m_data += static_cast<char>(unicode);
    }
    else if (unicode <= 0x7FF) 
    { // 双字节
        m_data += static_cast<char>(0xC0 | ((unicode >> 6) & 0x1F));
        m_data += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    else if (unicode <= 0xFFFF) 
    { // 三字节
        m_data += static_cast<char>(0xE0 | ((unicode >> 12) & 0x0F));
        m_data += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        m_data += static_cast<char>(0x80 | (unicode & 0x3F));
    }
    else if (unicode <= 0x10FFFF) 
    { // 四字节
        m_data += static_cast<char>(0xF0 | ((unicode >> 18) & 0x07));
        m_data += static_cast<char>(0x80 | ((unicode >> 12) & 0x3F));
        m_data += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        m_data += static_cast<char>(0x80 | (unicode & 0x3F));
    }
}

Char::operator bool() const
{
    return !m_data.empty() && Unicode() != 0;
}

uint32 Char::Unicode() const
{
    if (m_data.empty()) return 0;
    const unsigned char* data = reinterpret_cast<const unsigned char*>(m_data.data());
    size_t len = m_data.size();

    // 单字节
    if (len == 1) return data[0];

    // 多字节处理
    if (len == 2) 
    {
        return ((data[0] & 0x1F) << 6) | (data[1] & 0x3F);
    }
    if (len == 3) 
    {
        return ((data[0] & 0x0F) << 12) |
            ((data[1] & 0x3F) << 6) |
            (data[2] & 0x3F);
    }
    if (len == 4) 
    {
        return ((data[0] & 0x07) << 18) |
            ((data[1] & 0x3F) << 12) |
            ((data[2] & 0x3F) << 6) |
            (data[3] & 0x3F);
    }
    return 0;
}

bool Char::IsValid() const
{
    if (m_data.empty()) return false;
    const unsigned char* data = reinterpret_cast<const unsigned char*>(m_data.data());
    size_t len = m_data.size();

    // 单字节ASCII
    if (len == 1) return (data[0] <= 0x7F);

    // 多字节字符验证
    if (len > 4) return false; // UTF-8最大4字节

    // 检查首字节格式
    if ((data[0] & 0xE0) == 0xC0 && len == 2) return true;
    if ((data[0] & 0xF0) == 0xE0 && len == 3) return true;
    if ((data[0] & 0xF8) == 0xF0 && len == 4) return true;

    return false;
}

bool Char::IsLower() const
{
    const uint32 code = Unicode();
    return  (code >= 0x0061 && code <= 0x007A) ||   // a-z
            (code >= 0x00E0 && code <= 0x00F6) ||   // 拉丁字母附加扩展
            (code >= 0x00F8 && code <= 0x00FF);     // 拉丁字母补充
}

bool Char::IsUpper() const
{
    const uint32 code = Unicode();
    return  (code >= 0x0041 && code <= 0x005A) ||   // A-Z
            (code >= 0x00C0 && code <= 0x00D6) ||   // 拉丁字母附加扩展
            (code >= 0x00D8 && code <= 0x00DF);     // 拉丁字母补充
}

bool Char::IsNull() const
{
    return m_data.empty();
}

bool Char::IsNumber() const
{
    const uint32 code = Unicode();
    return code >= 0x0030 && code <= 0x0039; // 0-9
}

bool Char::IsPrint() const
{
    const uint32 code = Unicode();
    return code >= 0x0020 && code <= 0x007E; // 可打印ASCII范围
}

bool Char::IsSpace() const
{
    const uint32 code = Unicode();
    return  (code == 0x0020) ||  // 空格
            (code == 0x0009) ||  // 水平制表符
            (code == 0x000A);    // 换行
}

Char Char::ToLower() const
{
    const uint32_t code = Unicode();
    // ASCII范围转换
    if (code >= 0x0041 && code <= 0x005A) 
    {
        return Char(static_cast<char>(code + 0x20));
    }
    // 扩展拉丁字母处理（示例范围）
    if (code >= 0x00C0 && code <= 0x00D6) 
    {
        return Char(static_cast<char>(code + 0x20));
    }
    return *this;
}

Char Char::ToUpper() const
{
    const uint32_t code = Unicode();
    if (code >= 0x0061 && code <= 0x007A) 
    {
        return Char(static_cast<char>(code - 0x20));
    }
    if (code >= 0x00E0 && code <= 0x00F6) 
    {
        return Char(static_cast<char>(code - 0x20));
    }
    return *this;
}
