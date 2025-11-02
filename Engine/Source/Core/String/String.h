#pragma once

#include "Core.h"
#include "Container/Array.h"
#include "String/Char.h"
#include "Memory/ByteArray.h"

class StringList;
class ByteArray;
class String
{
public:
    friend class Char;
public:
    // 默认构造函数
    String() = default;

    // 析构函数
    ~String() = default;

    // 拷贝构造函数
    String(const String& other);

    // 拷贝赋值
    String& operator=(const String& other);

    // 移动构造函数
    String(String&& other);

    // 移动赋值
    String& operator=(String&& other);

    // C 风格字符串构造
    String(const char* str);

    // C 风格字符串赋值
    String& operator=(const char* str);

    // C++ 字符串构造
    String(const std::string& str);

    // C++ 字符串赋值
    String& operator=(const std::string& str);

    // 字节数组构造
    String(const ByteArray& buffer);

    // 字节数组赋值
    String& operator=(const ByteArray& buffer);

    // 隐式转化为 const std::string
    operator const std::string() const;

    // 隐式转换为 C 风格字符串
    operator const char* () const;

    // 隐式转换问 bool 值
    operator bool() const;
public:
    // 将字符串附加到末尾
    String& Append(const String& str);

    // 将字符附加到末尾
    String& Append(const Char& ch);

    // 将字符串添加到开头
    String& Prepend(const String& str);

    // 将字符添加到开头
    String& Prepend(const Char& ch);

    // 比较两个字符串
    int64 Compare(const String& str) const;

    // 判断字符串是否包含有给定的子串 
    bool Contains(const String& str) const;

    // 判断字符串是否包含有给定的字符
    bool Contains(const Char& ch) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int64 IndexOf(const String& str) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int64 IndexOf(const Char& ch) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int64 LastIndexOf(const String& str) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int64 LastIndexOf(const Char& ch) const;

    // 判断字符串是否有给定前缀
    bool StartWith(const String& str) const;

    // 判断字符串是否有给定后缀
    bool EndWith(const String& str) const;

    // 从字符串起始移除 count 个字符
    String& RemoveLeft(int64 count);

    // 从字符串中间移除 count 个字符
    String& RemoveMid(int64 index, int64 count);

    // 从字符串末尾移除 count 个字符
    String& RemoveRight(int64 count);

    // 保留字符串最左侧的 count 个字符,其余部分舍弃掉
    String& RemianLeft(int64 count);

    // 保留字符串从 index 位置开始的 count 个字符,其余部分舍弃掉
    String& RemianMid(int64 index, int64 count);

    // 保留字符串最右侧的 count 个字符,其余部分舍弃掉
    String& RemianRight(int64 count);

    // 返回一个字符串最左侧的 count 个字符的子串
    String LeftStr(int64 count);

    // 返回一个字符串从 index 位置开始的 count 个字符的子串
    String MidStr(int64 index, int64 count);

    // 返回一个字符串从 (start, end) 的子串,不包括 start 和 end
    String SubStr(int64 start, int64 end);

    // 返回一个字符串最右侧的 count 个字符的子串
    String RightStr(int64 count);

    // 移除字符串左右两侧的空白字符
    String& Trim();

    // 移除字符串左侧的空白字符
    String& TrimLeft();

    // 移除字符串右侧的空白字符
    String& TrimRight();

    // 返回以 sep 分割符分割的字符串中的所有部分
    StringList Split(char sep) const;

    // 用于将正则表达式 regexp 与字符串匹配
    String Match(const String& regex);

    // 向指定位置插入字符串
    String& Insert(int64 index, const String& str);

    // 替换字符串的指定部分
    String& Replace(int64 index, int64 count, const String& str);

    // 使当前字符串重复 n 次
    String& Repeat(int64 count);

    // 清空字符串
    void Clear();

    // 返回字符串预分配空间的大小
    int64 Capacity() const;

    // 返回字符串的大小
    int64 Size() const;

    // 返回字符串的字符数量
    int64 Count() const;

    // 预分配内存大小
    void Reserve(int64 size);

    // 移除未使用的容量
    void Shrink();

    // 判断字符串是否为空
    bool IsEmpty() const;

    // 判断字符串是否仅包含数字字符
    bool IsNumeric() const;

    // 判断索引是否为有效值
    bool IsValid(int64 index) const;

    // 交换两个字符串
    void Swap(String& other);

    // 将字符串转换为 int8 类型的值
    bool ToBool() const;

    // 将字符串转换为 int32 类型的值
    int32 ToInt32(int base = 10) const;

    // 将字符串转换为 int32 类型的值
    int64 ToInt64(int base = 10) const;

    // 将字符串转换为 uint32 类型的值
    uint32 ToUInt32(int base = 10) const;

    // 将字符串转换为 uint64 类型的值
    uint64 ToUInt64(int base = 10) const;

    // 将字符串转换为 float 类型的值
    float ToFloat() const;

    // 将字符串转换为 double 类型的值
    double ToDouble() const;

    // 将字符串转换为 C++ 类型的字符串
    std::string ToStdString() const;

    // 将字符串转换为 C 类型的字符串
    const char* ToCString() const;
public:
    Char operator[](int64 index);  
public:
    // 空字符串
    inline static const char* Empty = "";
private:
    // 计算 UTF-8 字符数量
    int64 _CalcCharCount(const char* str) const
    {
        if (!str || *str == '\0') return 0;

        int64 count = 0;
        const char* ptr = str;

        // UTF-8 字符首字节判断
        while (*ptr) 
        {   
            unsigned char ch = static_cast<unsigned char>(*ptr);

            // 单字节字符: 0xxxxxxx
            if ((ch & 0x80) == 0) 
            {         
                ptr += 1;
            }
            // 双字节字符: 110xxxxx
            else if ((ch & 0xE0) == 0xC0) 
            { 
                ptr += 2;
            }
            // 三字节字符: 1110xxxx
            else if ((ch & 0xF0) == 0xE0) 
            { 
                ptr += 3;
            }
            // 四字节字符: 11110xxx
            else if ((ch & 0xF8) == 0xF0) 
            { 
                ptr += 4;
            }
            // 无效UTF-8序列，按单字节处理
            else 
            {
                ptr += 1;
            }
            count++;
        }
        return count;
    }
    // 计算 UTF-8 字符数量
    int64 _CalcCharCount(const std::string str) const
    {
        if (str.empty()) return 0;

        int64 count = 0;
        const char* ptr = str.c_str();

        // UTF-8 字符首字节判断
        while (*ptr)
        {
            unsigned char ch = static_cast<unsigned char>(*ptr);

            // 单字节字符: 0xxxxxxx
            if ((ch & 0x80) == 0)
            {
                ptr += 1;
            }
            // 双字节字符: 110xxxxx
            else if ((ch & 0xE0) == 0xC0)
            {
                ptr += 2;
            }
            // 三字节字符: 1110xxxx
            else if ((ch & 0xF0) == 0xE0)
            {
                ptr += 3;
            }
            // 四字节字符: 11110xxx
            else if ((ch & 0xF8) == 0xF0)
            {
                ptr += 4;
            }
            // 无效UTF-8序列，按单字节处理
            else
            {
                ptr += 1;
            }
            count++;
        }
        return count;
    }
    // 计算从开始到该字节位置的字符数
    int64 _CalcCharCount(const char* str, int64 pos) const
    {
        if (!str || *str == '\0') return 0;

        int64 count = 0;
        const char* ptr = str;
        const char* end = ptr + pos;

        // UTF-8 字符首字节判断
        while (ptr < end)
        {
            unsigned char ch = static_cast<unsigned char>(*ptr);
            int32 size = 0;

            // 单字节字符: 0xxxxxxx
            if ((ch & 0x80) == 0)
            {
                size = 1;
            }
            // 双字节字符: 110xxxxx
            else if ((ch & 0xE0) == 0xC0)
            {
                size = 2;
            }
            // 三字节字符: 1110xxxx
            else if ((ch & 0xF0) == 0xE0)
            {
                size = 3;
            }
            // 四字节字符: 11110xxx
            else if ((ch & 0xF8) == 0xF0)
            {
                size = 4;
            }
            // 无效UTF-8序列，按单字节处理
            else
            {
                size = 1;
            }

            // 确保不超出目标字节位置
            if (ptr + size > end)
            {
                break;
            }

            ptr += size;
            count++;
        }
        return count;
    }
    // 计算从开始到该字节位置的字符数
    int64 _CalcCharCount(const std::string str, int64 pos) const
    {    
        if (str.empty()) return 0;

        int64 count = 0;
        const char* ptr = str.c_str();
        const char* end = ptr + pos;

        // UTF-8 字符首字节判断
        while (ptr < end)
        {      
            unsigned char ch = static_cast<unsigned char>(*ptr);
            int32 size = 0;

            // 单字节字符: 0xxxxxxx
            if ((ch & 0x80) == 0)
            {
                size = 1;
            }
            // 双字节字符: 110xxxxx
            else if ((ch & 0xE0) == 0xC0)
            {
                size = 2;
            }
            // 三字节字符: 1110xxxx
            else if ((ch & 0xF0) == 0xE0)
            {
                size = 3;
            }
            // 四字节字符: 11110xxx
            else if ((ch & 0xF8) == 0xF0)
            {
                size = 4;
            }
            // 无效UTF-8序列，按单字节处理
            else
            {
                size = 1;
            }

            // 确保不超出目标字节位置
            if (ptr + size > end) 
            {
                break;
            }

            ptr += size;
            count++;
        }
        return count;
    }
    // 将字符索引转换为字节位置
    int64 _IndexToPos(const std::string str, int64 index) const
    {
        if (index < 0 || index > m_count) return -1;

        if (index == 0) return 0;
        if (index == m_count) return str.size();

        const char* ptr = str.c_str();
        int64 pos = 0;
        int64 skip = 0;

        while (skip < index && *ptr != '\0')
        {
            unsigned char ch = static_cast<unsigned char>(*ptr);
            int64 size = 0;

            if ((ch & 0x80) == 0)
            {
                size = 1;
            }
            else if ((ch & 0xE0) == 0xC0)
            {
                size = 2;
            }
            else if ((ch & 0xF0) == 0xE0)
            {
                size = 3;
            }
            else if ((ch & 0xF8) == 0xF0)
            {
                size = 4;
            }
            else
            {
                size = 1;
            }

            pos += size;
            ptr += size;
            skip++;
        }

        return pos;
    }
private:
    // UTF-8 字符串
    std::string m_data;
    // Unicode 字符数量
    int64 m_count;
};

using StringList = Array<String>;