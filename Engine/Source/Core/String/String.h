#pragma once

#include "Core/Core.h"
class Char{};
class StringList;
class String
{
public:
    using iterator = std::string::iterator;
    using const_iterator = std::string::const_iterator;
    using reverse_iterator = std::string::reverse_iterator;
    using const_reverse_iterator = std::string::const_reverse_iterator;
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

    // 隐式转化为 std::string
    operator std::string() const;

    // 隐式转化为 const std::string
    operator const std::string() const;

    // 隐式转换为 C 风格字符串
    operator const char* () const;
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
    int32 Compare(const String& str) const;

    // 判断字符串是否包含有给定的子串 
    bool Contains(const String& str) const;

    // 判断字符串是否包含有给定的字符
    bool Contains(const Char& ch) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int32 IndexOf(const String& str) const;

    // 返回给定子串在字符串中首次出现的索引位置
    int32 IndexOf(const Char& ch) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int32 LastIndexOf(const String& str) const;

    // 返回给定字符在字符串中最后出现的索引位置
    int32 LastIndexOf(const Char& ch) const;

    // 判断字符串是否有给定前缀
    bool StartWith(const String& str) const;

    // 判断字符串是否有给定后缀
    bool EndWith(const String& str) const;

    // 从字符串起始移除 n 个字符
    String& RemoveLeft(int32 n);

    // 从字符串中间移除 n 个字符
    String& RemoveMid(int32 index, int32 n);

    // 从字符串末尾移除 n 个字符
    String& RemoveRight(int32 n);

    // 保留字符串最左侧的 n 个字符,其余部分舍弃掉
    String& RemianLeft(int32 n);

    // 保留字符串从 index 位置开始的 n 个字符,其余部分舍弃掉
    String& RemianMid(int32 index, int32 n);

    // 保留字符串最右侧的 n 个字符,其余部分舍弃掉
    String& RemianRight(int32 n);

    // 返回一个字符串最左侧的 n 个字符的子串
    String LeftStr(int32 n);

    // 返回一个字符串从 index 位置开始的 n 个字符的子串
    String MidStr(int32 index, int32 n);

    // 返回一个字符串从 (start, end) 的子串,不包括 start 和 end
    String SubStr(int64 start, int64 end);

    // 返回一个字符串最右侧的 n 个字符的子串
    String RightStr(int32 n);

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
    String& Insert(int32 index, const String& str);

    // 替换字符串的指定部分
    String& Replace(int32 index, int32 n, const String& str);

    // 使当前字符串重复 n 次
    String& Repeat(int32 n);

    // 清空字符串
    void Clear();

    // 返回字符串预分配空间的大小
    int32 Capacity() const;

    // 返回字符串的大小
    int32 Size() const;

    // 返回字符串的字符数量
    int32 Count() const;

    // 预分配内存大小
    void Reserve(int32 size);

    // 移除未使用的容量
    void Shrink();

    // 判断字符串是否为空
    bool IsEmpty() const;

    // 判断字符串是否仅包含数字字符
    bool IsNumeric() const;

    // 判断索引是否为有效值
    bool IsValid(int32 index) const;

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
    // 空字符串
    inline static const char* Empty = "";
private:
    // UTF-8 字符串
    std::string m_data;
    // Unicode 字符数量
    int32 m_count;
};