#include "pch.h"

#include "String.h"

#include <regex>

/* static */
String String::FromStdString(const std::string& str)
{
    return String(str);
}

String String::FromCString(const char* str)
{
    return String(str);
}

String String::FromInt8(int8 value)
{
    return String(std::to_string(static_cast<int>(value)));
}

String String::FromInt16(int16 value)
{
    return String(std::to_string(static_cast<int>(value)));
}

String String::FromInt32(int32 value)
{
    return String(std::to_string(value));
}

String String::FromInt64(int64 value)
{
    return String(std::to_string(value));
}

String String::FromUInt8(uint8 value)
{
    return String(std::to_string(static_cast<unsigned int>(value)));
}

String String::FromUInt16(uint16 value)
{
    return String(std::to_string(static_cast<unsigned int>(value)));
}

String String::FromUInt32(uint32 value)
{
    return String(std::to_string(value));
}

String String::FromUInt64(uint64 value)
{
    return String(std::to_string(value));
}

String String::FromFloat(float value)
{
    std::string str = std::to_string(value);
    // 移除尾随零
    size_t dotPos = str.find('.');
    if (dotPos != std::string::npos)
    {
        size_t lastNonZero = str.find_last_not_of('0');
        if (lastNonZero != std::string::npos)
        {
            if (str[lastNonZero] == '.')
            {
                str.erase(lastNonZero);
            }
            else
            {
                str.erase(lastNonZero + 1);
            }
        }
    }
    return String(str);
}

String String::FromDouble(double value)
{
    std::string str = std::to_string(value);
    // 移除尾随零
    size_t dotPos = str.find('.');
    if (dotPos != std::string::npos)
    {
        size_t lastNonZero = str.find_last_not_of('0');
        if (lastNonZero != std::string::npos)
        {
            if (str[lastNonZero] == '.')
            {
                str.erase(lastNonZero);
            }
            else
            {
                str.erase(lastNonZero + 1);
            }
        }
    }
    return String(str);
}

String String::FromBool(bool value)
{
    return String(value ? "true" : "false");
}

/* member */
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

String::operator std::filesystem::path() const
{
    return std::filesystem::path(m_data);
}

String::operator const std::string() const
{
    return m_data;
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
    m_count += _CalcCharCount(str.m_data);  // 修复：没有更新m_count
    return *this;
}

String& String::Prepend(const Char& ch)
{
    m_data.insert(0, ch.m_data);
    m_count += 1;
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
    // �ж������ַ����Ƿ�Ϊ��
    if (!str)
    {
        return -1;
    }
    // �����Ӵ����ֽ�λ��
    int64 pos = m_data.find(str.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // ����ӿ�ʼ�����ֽ�λ�õ��ַ���
    return _CalcCharCount(m_data, pos);
}

int64 String::IndexOf(const Char& ch) const
{
    // �ж������ַ����Ƿ�Ϊ��
    if (!ch)
    {
        return -1;
    }
    // �����Ӵ����ֽ�λ��
    int64 pos = m_data.find(ch.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // ����ӿ�ʼ�����ֽ�λ�õ��ַ���
    return _CalcCharCount(m_data, pos);
}

int64 String::LastIndexOf(const String& str) const
{
    // �ж������ַ����Ƿ�Ϊ��
    if (!str)
    {
        return -1;
    }
    // �����Ӵ����ֽ�λ��
    int64 pos = m_data.rfind(str.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // ����ӿ�ʼ�����ֽ�λ�õ��ַ���
    return _CalcCharCount(m_data, pos);
}

int64 String::LastIndexOf(const Char& ch) const
{
    // �ж������ַ����Ƿ�Ϊ��
    if (!ch)
    {
        return -1;
    }
    // �����Ӵ����ֽ�λ��
    int64 pos = m_data.rfind(ch.m_data);
    if (pos == std::string::npos)
    {
        return -1;
    }
    // ����ӿ�ʼ�����ֽ�λ�õ��ַ���
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

    // ���ɾ���ַ��������ڵ����ַ������ַ���������ַ���
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

    // ����ɾ��������ֹԽ��
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
    TrimLeft();
    TrimRight();
    return *this;
}

String& String::TrimLeft()
{
    // 使用UTF-8安全的空白字符判断
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(m_data.data());
    size_t len = m_data.size();
    size_t start = 0;

    while (start < len)
    {
        unsigned char ch = ptr[start];

        // 检查是否为空白字符（ASCII和Unicode）
        bool isSpace = false;

        // ASCII空白字符
        if (ch == 0x20 || ch == 0x09 || ch == 0x0A || ch == 0x0D || ch == 0x0C || ch == 0x0B)
        {
            isSpace = true;
            start += 1;
        }
        // UTF-8多字节空白字符（如不换行空格）
        else if ((ch & 0xE0) == 0xC0 && len - start >= 2)
        {
            uint32 code = ((ptr[start] & 0x1F) << 6) | (ptr[start + 1] & 0x3F);
            if (code == 0x00A0)  // 不换行空格
            {
                isSpace = true;
                start += 2;
            }
        }
        else if ((ch & 0xF0) == 0xE0 && len - start >= 3)
        {
            uint32 code = ((ptr[start] & 0x0F) << 12) |
                ((ptr[start + 1] & 0x3F) << 6) |
                (ptr[start + 2] & 0x3F);
            if (code == 0x3000 ||  // 表意空格
                code == 0x2000 || code == 0x2001 || code == 0x2002 ||
                code == 0x2003 || code == 0x2004 || code == 0x2005 ||
                code == 0x2006 || code == 0x2007 || code == 0x2008 ||
                code == 0x2009 || code == 0x200A || code == 0x202F ||
                code == 0x205F)
            {
                isSpace = true;
                start += 3;
            }
        }

        if (!isSpace)
        {
            break;
        }
    }

    if (start > 0)
    {
        m_data.erase(0, start);
        m_count = _CalcCharCount(m_data);
    }

    return *this;
}

String& String::TrimRight()
{
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(m_data.data());
    size_t len = m_data.size();
    size_t end = len;

    while (end > 0)
    {
        // 找到字符起始位置
        size_t charStart = end - 1;
        while (charStart > 0 && (ptr[charStart] & 0xC0) == 0x80)
        {
            charStart--;
        }

        unsigned char ch = ptr[charStart];
        size_t charLen = end - charStart;
        bool isSpace = false;

        // ASCII空白字符
        if (ch == 0x20 || ch == 0x09 || ch == 0x0A || ch == 0x0D || ch == 0x0C || ch == 0x0B)
        {
            isSpace = true;
        }
        // UTF-8多字节空白字符
        else if ((ch & 0xE0) == 0xC0 && charLen >= 2)
        {
            uint32 code = ((ptr[charStart] & 0x1F) << 6) | (ptr[charStart + 1] & 0x3F);
            if (code == 0x00A0) isSpace = true;
        }
        else if ((ch & 0xF0) == 0xE0 && charLen >= 3)
        {
            uint32 code = ((ptr[charStart] & 0x0F) << 12) |
                ((ptr[charStart + 1] & 0x3F) << 6) |
                (ptr[charStart + 2] & 0x3F);
            if (code == 0x3000 || (code >= 0x2000 && code <= 0x200A) ||
                code == 0x202F || code == 0x205F)
            {
                isSpace = true;
            }
        }

        if (!isSpace)
        {
            break;
        }

        end = charStart;
    }

    if (end < len)
    {
        m_data.erase(end);
        m_count = _CalcCharCount(m_data);
    }

    return *this;
}

StringList String::Split(char sep) const
{
    StringList result;
    std::stringstream ss(m_data);
    std::string str;

    while (std::getline(ss, str, sep))
    {
        result.Push(str);
    }

    return result;
}

String String::Match(const String& regex)
{
    try
    {
        std::regex pattern(regex.m_data);
        std::smatch match;

        if (std::regex_search(m_data, match, pattern))
        {
            return String(match[0].str());
        }
    }
    catch (const std::regex_error&)
    {
        // 正则表达式无效，返回空字符串
    }

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
            hasDigit = false;
        }
        else if ((c == '+' || c == '-') && (i == 0 || m_data[i - 1] == 'e' || m_data[i - 1] == 'E'))
        {

        }
        else {
            return false;
        }
    }

    return hasDigit;
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
    if (m_data == "true" || m_data == "True")
    {
        return true;
    }
    else if (m_data == "false" || m_data == "False")
    {
        return false;
    }
    else
    {
        throw std::invalid_argument("No digits were found");
    }
}

int8 String::ToInt8(int base) const
{
    int8 result;
    char* endptr;

    result = static_cast<int8>(std::strtol(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

int16 String::ToInt16(int base) const
{
    int16 result;
    char* endptr;

    result = static_cast<int16>(std::strtol(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

int32 String::ToInt32(int base) const
{
    int32 result;
    char* endptr;

    result = static_cast<int32>(std::strtol(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

int64 String::ToInt64(int base) const
{
    if (m_data.empty())
    {
        throw std::invalid_argument("Cannot convert empty string to integer");
    }

    char* endptr;
    errno = 0;
    int64 result = std::strtoll(m_data.c_str(), &endptr, base);

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    if (errno == ERANGE)
    {
        throw std::out_of_range("Integer conversion out of range");
    }

    return result;
}

uint8 String::ToUInt8(int base) const
{
    uint8 result;
    char* endptr;

    result = static_cast<uint8>(std::strtoul(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

uint16 String::ToUInt16(int base) const
{
    uint16 result;
    char* endptr;

    result = static_cast<uint16>(std::strtoul(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

uint32 String::ToUInt32(int base) const
{
    uint32 result;
    char* endptr;

    result = static_cast<uint32>(std::strtoul(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    return result;
}

uint64 String::ToUInt64(int base) const
{
    if (m_data.empty())
    {
        throw std::invalid_argument("Cannot convert empty string to unsigned integer");
    }

    char* endptr;
    errno = 0;
    uint64 result = std::strtoull(m_data.c_str(), &endptr, base);

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
    }

    if (errno == ERANGE)
    {
        throw std::out_of_range("Unsigned integer conversion out of range");
    }

    return result;
}

float String::ToFloat() const
{
    char* endptr;
    float result = std::strtof(m_data.c_str(), &endptr);

    if (endptr == m_data.c_str() || *endptr != '\0')
    {
        throw std::invalid_argument("Invalid float conversion");
    }

    return result;
}

double String::ToDouble() const
{
    char* endptr;
    double result = std::strtod(m_data.c_str(), &endptr);

    if (endptr == m_data.c_str() || *endptr != '\0')
    {
        throw std::invalid_argument("Invalid float conversion");
    }

    return result;
}

std::string String::ToStdString() const
{
    return m_data;
}

const char* String::ToCString() const
{
    return m_data.c_str();
}

Char String::operator[](int64 index)
{
    if (!IsValid(index))
    {
        return Char();  // 返回空字符
    }

    int64 bytePos = _IndexToPos(m_data, index);
    return Char(_GetCharAtBytePos(m_data, bytePos));
}

String& String::operator+=(const String& str)
{
    m_data += str.m_data;
    m_count += str.m_count;  // 修复：没有更新m_count
    return *this;
}

String& String::operator+=(char ch)
{
    m_data += ch;
    m_count += 1;  // 修复：没有更新m_count
    return *this;
}

String operator+(const String& left, const String& right)
{
    String str(left);
    str.Reserve(left.Size() + right.Size() + 1);
    str += right;
    return str;
}

String operator+(const String& left, const char* right)
{
    return String(left.m_data + right);
}

String operator+(const char* left, const String& right)
{
    return String(left + right.m_data);
}

bool operator==(const String& left, const String& right)
{
    return left.m_data == right.m_data;
}

bool operator==(const String& left, const char* right)
{
    return left.m_data == right;
}

bool operator==(const char* left, const String& right)
{
    return left == right.m_data;
}

bool operator!=(const String& left, const String& right)
{
    return left.m_data != right.m_data;
}

bool operator!=(const String& left, const char* right)
{
    return left.m_data != right;
}

bool operator!=(const char* left, const String& right)
{
    return left != right.m_data;
}

bool operator>(const String& left, const String& right)
{
    return left.m_data > right.m_data;
}

bool operator>(const String& left, const char* right)
{
    return left.m_data > right;
}

bool operator>(const char* left, const String& right)
{
    return left > right.m_data;
}

bool operator>=(const String& left, const String& right)
{
    return left.m_data >= right.m_data;
}

bool operator>=(const String& left, const char* right)
{
    return left.m_data >= right;
}

bool operator>=(const char* left, const String& right)
{
    return left >= right.m_data;
}

bool operator<(const String& left, const String& right)
{
    return left.m_data < right.m_data;
}

bool operator<(const String& left, const char* right)
{
    return left.m_data < right;
}

bool operator<(const char* left, const String& right)
{
    return left < right.m_data;
}

bool operator<=(const String& left, const String& right)
{
    return left.m_data <= right.m_data;
}

bool operator<=(const String& left, const char* right)
{
    return left.m_data < right;
}

bool operator<=(const char* left, const String& right)
{
    return left < right.m_data;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << str.ToStdString();
}



/* private */
int32 String::_GetCharLength(unsigned char firstByte) const
{
    if ((firstByte & 0x80) == 0) return 1;      // 0xxxxxxx
    if ((firstByte & 0xE0) == 0xC0) return 2;   // 110xxxxx
    if ((firstByte & 0xF0) == 0xE0) return 3;   // 1110xxxx
    if ((firstByte & 0xF8) == 0xF0) return 4;   // 11110xxx
    return 1;                                   // 无效编码，按单字节处理
}

bool String::_IsValidTrailByte(unsigned char byte) const
{
    return (byte & 0xC0) == 0x80;  // 10xxxxxx
}

int64 String::_CalcCharCount(const char* str) const
{
    if (!str) return 0;

    int64 count = 0;
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str);

    while (*ptr)
    {
        int32 len = _GetCharLength(*ptr);

        // 验证多字节字符的后续字节
        if (len > 1)
        {
            bool valid = true;
            for (int32 i = 1; i < len; ++i)
            {
                if (ptr[i] == '\0' || !_IsValidTrailByte(ptr[i]))
                {
                    valid = false;
                    break;
                }
            }
            if (!valid) len = 1;  // 无效序列，按单字节处理
        }

        ptr += len;
        count++;
    }

    return count;
}

int64 String::_CalcCharCount(const std::string& str) const
{
    return _CalcCharCount(str.c_str());
}

int64 String::_CalcCharCount(const char* str, int64 bytePos) const
{
    if (!str || bytePos <= 0) return 0;

    int64 count = 0;
    int64 currentPos = 0;
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str);

    while (currentPos < bytePos && *ptr)
    {
        int32 len = _GetCharLength(*ptr);

        // 确保不超出目标字节位置
        if (currentPos + len > bytePos)
        {
            break;
        }

        // 验证多字节字符
        if (len > 1)
        {
            bool valid = true;
            for (int32 i = 1; i < len; ++i)
            {
                if (ptr[i] == '\0' || !_IsValidTrailByte(ptr[i]))
                {
                    valid = false;
                    break;
                }
            }
            if (!valid) len = 1;
        }

        ptr += len;
        currentPos += len;
        count++;
    }

    return count;
}

int64 String::_CalcCharCount(const std::string& str, int64 bytePos) const
{
    if (bytePos > static_cast<int64>(str.size()))
    {
        bytePos = static_cast<int64>(str.size());
    }
    return _CalcCharCount(str.c_str(), bytePos);
}

int64 String::_IndexToPos(const std::string& str, int64 index) const
{
    // 边界检查
    if (index < 0) return 0;
    if (index >= m_count) return static_cast<int64>(str.size());
    if (index == 0) return 0;

    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str.data());
    int64 bytePos = 0;
    int64 charCount = 0;
    size_t strLen = str.size();

    while (charCount < index && bytePos < static_cast<int64>(strLen))
    {
        int32 len = _GetCharLength(ptr[bytePos]);

        // 验证多字节字符
        if (len > 1)
        {
            bool valid = true;
            for (int32 i = 1; i < len; ++i)
            {
                if (bytePos + i >= static_cast<int64>(strLen) ||
                    !_IsValidTrailByte(ptr[bytePos + i]))
                {
                    valid = false;
                    break;
                }
            }
            if (!valid) len = 1;
        }

        bytePos += len;
        charCount++;
    }

    return bytePos;
}

std::string String::_GetCharAtBytePos(const std::string& str, int64 bytePos) const
{
    if (bytePos < 0 || bytePos >= static_cast<int64>(str.size()))
    {
        return std::string();
    }

    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str.data()) + bytePos;
    int32 len = _GetCharLength(*ptr);

    // 验证完整字符
    if (bytePos + len > static_cast<int64>(str.size()))
    {
        len = static_cast<int32>(str.size() - bytePos);
    }
    else if (len > 1)
    {
        for (int32 i = 1; i < len; ++i)
        {
            if (!_IsValidTrailByte(ptr[i]))
            {
                len = 1;  // 无效序列，只取首字节
                break;
            }
        }
    }

    return str.substr(bytePos, len);
}

bool String::_IsValidUTF8String(const std::string& str) const
{
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str.data());
    size_t len = str.size();
    size_t pos = 0;

    while (pos < len)
    {
        int32 charLen = _GetCharLength(ptr[pos]);

        // 检查长度是否超出字符串
        if (pos + charLen > len)
        {
            return false;
        }

        // 验证多字节字符
        if (charLen > 1)
        {
            // 检查首字节是否有效
            if (charLen == 2 && (ptr[pos] & 0xFE) == 0xC0)  // 过长的C0和C1
                return false;
            if (charLen == 3 && ptr[pos] == 0xE0 && ptr[pos + 1] < 0xA0)  // 过长的3字节
                return false;
            if (charLen == 4 && ptr[pos] == 0xF0 && ptr[pos + 1] < 0x90)  // 过长的4字节
                return false;
            if (charLen == 4 && ptr[pos] == 0xF4 && ptr[pos + 1] > 0x8F)  // 超出U+10FFFF
                return false;

            // 验证后续字节
            for (int32 i = 1; i < charLen; ++i)
            {
                if (!_IsValidTrailByte(ptr[pos + i]))
                {
                    return false;
                }
            }
        }

        pos += charLen;
    }

    return true;
}

int64 String::_NextCharPos(const std::string& str, int64 currentPos) const
{
    if (currentPos < 0 || currentPos >= static_cast<int64>(str.size()))
    {
        return -1;
    }

    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str.data());
    int32 len = _GetCharLength(ptr[currentPos]);

    return currentPos + len;
}

int64 String::_PrevCharPos(const std::string& str, int64 currentPos) const
{
    if (currentPos <= 0 || currentPos > static_cast<int64>(str.size()))
    {
        return -1;
    }

    int64 pos = currentPos - 1;
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(str.data());

    // 向后找到字符的首字节
    while (pos > 0 && (ptr[pos] & 0xC0) == 0x80)
    {
        pos--;
    }

    return pos;
}