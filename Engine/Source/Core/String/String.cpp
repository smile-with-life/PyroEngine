#include "pch.h"

#include "String.h"
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
    return *this;
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
    return *this;
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
        result.Push(str);
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
    int64 result;
    char* endptr;

    result = static_cast<int64>(std::strtol(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
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
    uint64 result;
    char* endptr;

    result = static_cast<uint64>(std::strtoul(m_data.c_str(), &endptr, base));

    if (endptr == m_data.c_str())
    {
        throw std::invalid_argument("No digits were found");
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

std::u16string String::ToUTF16() const
{
    return std::u16string();
}

Char String::operator[](int64 index)
{
    return Char();
}

String& String::operator+=(const String& str)
{
    m_data += str.m_data;
    return *this;
}

String& String::operator+=(char ch)
{
    m_data += ch;
    return *this;
}

String operator+(const String& left, const String& right)
{
    String str(left);
    str.Reserve(left.Size() + right.Size() + 1);
    str += right;
    return str;
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

/* static */
String String::FromStdString(const std::string& str)
{
    return String();
}

String String::FromCString(const char* str)
{
    return String();
}

String String::FromUTF16(std::u16string u16str)
{
    return String();
}

String String::FromInt8(int8 value)
{
    return String();
}

String String::FromInt16(int16 value)
{
    return String();
}

String String::FromInt32(int32 value)
{
    return String();
}

String String::FromInt64(int64 value)
{
    return String();
}

String String::FromUInt8(uint8 value)
{
    return String();
}

String String::FromUInt16(uint16 value)
{
    return String();
}

String String::FromUInt32(uint32 value)
{
    return String();
}

String String::FromUInt64(uint64 value)
{
    return String();
}

String String::FromFloat(float value)
{
    return String();
}

String String::FromDouble(double value)
{
    return String();
}

String String::FromBool(bool value)
{
    return String();
}
