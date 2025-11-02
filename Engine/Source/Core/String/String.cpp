#include "pch.h"

#include "String.h"

String::String(const String& other)
    : m_data(other.m_data)
    , m_count(other.m_count)
{

}

String& String::operator=(const String& other)
{
    if (this == &other) [[unlikely]]
    {
        return *this;
    }

    m_data = other.m_data;
    m_count = other.m_count;
    return *this;
}

String::String(String&& other)
    : m_data(std::move(other.m_data))
    , m_count(other.m_count)
{
    other.m_count = 0;
}

String& String::operator=(String&& other)
{
    if (this == &other) [[unlikely]]
    {
        return *this;
    }

    m_data = std::move(other.m_data);
    m_count = other.m_count;

    other.m_count = 0;
    return *this;
}

String::String(const char* str)
    : m_data(str)
{
    m_count = _CalcCharCount(str);
}

String& String::operator=(const char* str)
{
    m_data = str;
    m_count = _CalcCharCount(str);
    return *this;
}

String::String(const std::string& str)
    : m_data(str)
{
    m_count = _CalcCharCount(str.c_str());
}

String& String::operator=(const std::string& str)
{
    m_data = str;
    m_count = _CalcCharCount(str.c_str());
    return *this;
}

String::String(const ByteArray& buffer)
{
    m_data.clear();

    for (int64 i = 0; i < buffer.Size(); ++i)
    {
        if (buffer[i] == '\0') break;
        m_data.push_back(buffer[i]);
    }
}

String& String::operator=(const ByteArray& buffer)
{
    m_data.clear();

    for (int64 i = 0; i < buffer.Size(); ++i)
    {
        if (buffer[i] == '\0') break;
        m_data.push_back(buffer[i]);
    }
    return *this;
}

String::operator const std::string() const
{
    return m_data;
}

String::operator const char* () const
{
    return m_data.c_str();
}

String::operator bool() const
{
    return !m_data.empty();
}

String& String::Append(const String& str)
{
    m_data.append(str.m_data);
    m_count += _CalcCharCount(str.m_data);
    return *this;
}

String& String::Append(const Char& ch)
{
    m_data.append(ch.m_data);
    m_count += 1;
    return *this;
}

String& String::Prepend(const String& str)
{
    m_data.insert(0, str.m_data);
    return *this;
}

String& String::Prepend(const Char& ch)
{
    m_data.insert(0, ch.m_data);
    return *this;
}

int64 String::Compare(const String& str) const
{
    return m_data.compare(str.m_data);
}

bool String::Contains(const String& str) const
{
    return m_data.contains(str.m_data);
}

bool String::Contains(const Char& ch) const
{
    return m_data.contains(ch.m_data);
}

int64 String::IndexOf(const String& str) const
{
    // 判断输入字符串是否为空
    if (!str)
    {  
        return -1;
    }
    // 查找子串的字节位置
    int64 pos = m_data.find(str.m_data);
    if (pos == std::string::npos) 
    {
        return -1;
    }
    // 计算从开始到该字节位置的字符数
    return _CalcCharCount(m_data, pos);
}

int64 String::IndexOf(const Char& ch) const
{
    // 判断输入字符串是否为空
    if (!ch)
    {
        return -1;
    }
    // 查找子串的字节位置
    int64 pos = m_data.find(ch.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // 计算从开始到该字节位置的字符数
    return _CalcCharCount(m_data, pos);
}

int64 String::LastIndexOf(const String& str) const
{
    // 判断输入字符串是否为空
    if (!str)
    {
        return -1;
    }
    // 查找子串的字节位置
    int64 pos = m_data.rfind(str.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // 计算从开始到该字节位置的字符数
    return _CalcCharCount(m_data, pos);
}

int64 String::LastIndexOf(const Char& ch) const
{
    // 判断输入字符串是否为空
    if (!ch)
    {
        return -1;
    }
    // 查找子串的字节位置
    int64 pos = m_data.rfind(ch.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // 计算从开始到该字节位置的字符数
    return _CalcCharCount(m_data, pos);
}

bool String::StartWith(const String& str) const
{
    return m_data.starts_with(str.m_data);
}

bool String::EndWith(const String& str) const
{
    return m_data.ends_with(str.m_data);
}

String& String::RemoveLeft(int64 count)
{
    if (count <= 0) return *this;

    // 如果删除字符数量大于等于字符串的字符数，清空字符串
    if (count >= m_count) 
    {
        Clear();
        m_count = 0;
        return *this;
    }

    int64 pos = _IndexToPos(m_data, count);
    if (pos > 0) 
    {
        m_data.erase(0, pos);
        m_count -= count;
    }

    return *this;
}

String& String::RemoveMid(int64 index, int64 count)
{
    if (count <= 0 || index >= m_count) return *this;

    // 调整删除数量防止越界
    if (index + count > m_count) 
    {
        count = m_count - index;
    }

    int64 start = _IndexToPos(m_data, index);
    int64 end = _IndexToPos(m_data, index + count);

    if (start >= 0 && end > start) 
    {
        m_data.erase(start, end - start);
        m_count -= count;
    }
    return *this;
}

String& String::RemoveRight(int64 count)
{
    if (count <= 0) return *this;

    if (count >= m_count) 
    {
        m_data.clear();
        m_count = 0;
        return *this;
    }

    int64 keepCounts = m_count - count;
    int64 pos = _IndexToPos(m_data, keepCounts);

    if (pos >= 0) 
    {
        m_data.erase(pos);
        m_count = keepCounts;
    }
    return *this;
}

String& String::RemianLeft(int64 count)
{
    if (count <= 0) 
    {
        m_data.clear();
        m_count = 0;
    }
    else if (count < m_count) 
    {
        int64 pos = _IndexToPos(m_data, count);
        if (pos > 0) 
        {
            m_data.erase(pos);
            m_count = count;
        }
    }
    return *this;
}

String& String::RemianMid(int64 index, int64 count)
{
    if (count <= 0 || index >= m_count) 
    {
        m_data.clear();
        m_count = 0;
        return *this;
    }

    if (index + count > m_count) 
    {
        count = m_count - index;
    }

    int64 start = _IndexToPos(m_data, index);
    int64 end = _IndexToPos(m_data, index + count);

    if (start >= 0 && end > start)
    {
        m_data = m_data.substr(start, end - start);
        m_count = count;
    }
    else 
    {
        m_data.clear();
        m_count = 0;
    }
    return *this;
}

String& String::RemianRight(int64 count)
{
    if (count <= 0) 
    {
        m_data.clear();
        m_count = 0;
    }
    else if (count < m_count) 
    {
        int64 start = _IndexToPos(m_data, m_count - count);
        if (start >= 0) 
        {
            m_data = m_data.substr(start);
            m_count = count;
        }
    }
    return *this;
}

String String::LeftStr(int64 count)
{
    if (count <= 0) return String("");
    if (count >= m_count) return *this;

    int64 pos = _IndexToPos(m_data, count);
    if (pos > 0)
    {
        return String(m_data.substr(0, pos).c_str());
    }
    return String("");
}

String String::MidStr(int64 index, int64 count)
{
    if (index < 0 || count <= 0 || index >= m_count) {
        return String("");
    }

    if (index + count > m_count) {
        count = m_count - index;
    }

    int64 start = _IndexToPos(m_data, index);
    int64 end = _IndexToPos(m_data, index + count);

    if (start >= 0 && end > start) 
    {
        return String(m_data.substr(start, end - start).c_str());
    }
    return String("");
}

String String::SubStr(int64 start, int64 end)
{
    if (start < 0 || end < start || start >= m_count) {
        return String("");
    }

    if (end > m_count) end = m_count;

    int64 startPos = _IndexToPos(m_data, start);
    int64 endPos = _IndexToPos(m_data, end);

    if (startPos >= 0 && endPos > startPos) {
        return String(m_data.substr(startPos, endPos - startPos).c_str());
    }
    return String("");
}

String String::RightStr(int64 count)
{
    if (count <= 0) return String("");
    if (count >= m_count) return *this;

    int64 startPos = _IndexToPos(m_data, m_count - count);
    if (startPos >= 0) 
    {
        return String(m_data.substr(startPos).c_str());
    }
    return String("");
}

String& String::Trim()
{
    
}

String& String::TrimLeft()
{
    auto iter = std::find_if_not(
        m_data.begin(),
        m_data.end(),
        [](unsigned char ch)
        {
            return std::isspace(ch);
        });
    m_data.erase(m_data.begin(), iter);
    m_count = _CalcCharCount(m_data.c_str());
}

String& String::TrimRight()
{
    auto iter = std::find_if_not(
        m_data.rbegin(), 
        m_data.rend(),
        [](unsigned char ch) 
        { 
            return std::isspace(ch); 
        });
    m_data.erase(iter.base(), m_data.end());
    m_count = _CalcCharCount(m_data.c_str());
    return *this;
}

StringList String::Split(char sep) const
{
    StringList result;
    std::stringstream ss(m_data);
    std::string str;

    while (std::getline(ss, str, sep))
    {
        result.Emplace(str);
    }

    return result;
}

String String::Match(const String& regex)
{
    return String();
}

String& String::Insert(int64 index, const String& str)
{
    if (index < 0 || index > m_count) {
        return *this;
    }

    int64 pos = _IndexToPos(m_data, index);
    if (pos >= 0) 
    {
        m_data.insert(pos, str.m_data);
        m_count += str.m_count;
    }
    return *this;
}

String& String::Replace(int64 index, int64 count, const String& str)
{
    if (index < 0 || count < 0 || index >= m_count) 
    {
        return *this;
    }

    if (index + count > m_count) 
    {
        count = m_count - index;
    }

    int64 startPos = _IndexToPos(m_data, index);
    int64 endPos = _IndexToPos(m_data, index + count);

    if (startPos >= 0 && endPos > startPos) 
    {
        m_data.replace(startPos, endPos - startPos, str.m_data);
        m_count = m_count - count + str.m_count;
    }
    return *this;
}

String& String::Repeat(int64 count)
{
    if (count <= 0) 
    {
        m_data.clear();
        m_count = 0;
    }
    else if (count > 1) 
    {
        std::string original = m_data;
        for (int64 i = 1; i < count; ++i)
        {
            m_data += original;
        }
        m_count = _CalcCharCount(m_data.c_str());
    }
    return *this;
}

void String::Clear()
{
    m_data.clear();
    m_count = 0;
}

int64 String::Capacity() const
{
    return m_data.capacity();
}

int64 String::Size() const
{
    return m_data.size();
}

int64 String::Count() const
{
    return m_count;
}

void String::Reserve(int64 size)
{
    m_data.reserve(size);
}

void String::Shrink()
{
    m_data.shrink_to_fit();
}

bool String::IsEmpty() const
{
    return m_data.empty();
}

bool String::IsNumeric() const
{
    if (m_data.empty()) return false;

    bool hasDigit = false;
    bool hasDot = false;
    bool hasE = false;

    for (int64 i = 0; i < m_data.size(); ++i) 
    {
        char c = m_data[i];

        if (c >= '0' && c <= '9') 
        {
            hasDigit = true;
        }
        else if (c == '.' && !hasDot && !hasE) 
        {
            hasDot = true;
        }
        else if ((c == 'e' || c == 'E') && hasDigit && !hasE) 
        {
            hasE = true;
            hasDigit = false; // e后面必须有数字
        }
        else if ((c == '+' || c == '-') && (i == 0 || m_data[i - 1] == 'e' || m_data[i - 1] == 'E')) 
        {
            // 符号只能在开头或e后面
        }
        else {
            return false;
        }
    }

    return hasDigit; // 必须至少有一个数字
}

bool String::IsValid(int64 index) const
{
    if (index < 0) return false;
    
    return index < m_count ? true : false;
}

void String::Swap(String& other)
{
    std::swap(m_data, other.m_data);
    std::swap(m_count, other.m_count);
}

bool String::ToBool() const
{
    return false;
}
int32 String::ToInt32(int base) const
{
    return int32();
}
int64 String::ToInt64(int base) const
{
    return int64();
}
uint32 String::ToUInt32(int base) const
{
    return uint32();
}
uint64 String::ToUInt64(int base) const
{
    return uint64();
}
float String::ToFloat() const
{
    return 0.0f;
}
double String::ToDouble() const
{
    return 0.0;
}
std::string String::ToStdString() const
{
    return std::string();
}
const char* String::ToCString() const
{
    return nullptr;
}