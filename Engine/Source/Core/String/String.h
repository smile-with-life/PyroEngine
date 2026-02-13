#pragma once

#include "Core.h"
#include "Container/Array.h"
#include "String/Char.h"
#include "Memory/ByteArray.h"

using StringList = Array<String>;

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

    // 从 C 风格字符串构造
    String(const char* str);

    // 从 C 风格字符串赋值
    String& operator=(const char* str);

    // 从 C++ 字符串构造
    String(const std::string& str);

    // 从 C++ 字符串赋值
    String& operator=(const std::string& str);

    // 从字节数组构造
    String(const ByteArray& buffer);

    // 从字节数组赋值
    String& operator=(const ByteArray& buffer);

    operator std::filesystem::path() const;

    // 隐式转化为 const std::string
    operator const std::string() const;

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

    // 将字符串转换为 int8 类型的值
    int8 ToInt8(int base = 10) const;

    // 将字符串转换为 int16 类型的值
    int16 ToInt16(int base = 10) const;

    // 将字符串转换为 int32 类型的值
    int32 ToInt32(int base = 10) const;

    // 将字符串转换为 int32 类型的值
    int64 ToInt64(int base = 10) const;

    // 将字符串转换为 uint8 类型的值
    uint8 ToUInt8(int base = 10) const;

    // 将字符串转换为 uint16 类型的值
    uint16 ToUInt16(int base = 10) const;

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

    String& operator+=(const String& str);

    String& operator+=(char ch);

    friend String operator+(const String& left, const String& right);

    friend String operator+(const String& left, const char* right);

    friend String operator+(const char* left, const String& right);

    friend bool operator==(const String& left, const String& right);

    friend bool operator==(const String& left, const char* right);

    friend bool operator==(const char* left, const String& right);

    friend bool operator!=(const String& left, const String& right);

    friend bool operator!=(const String& left, const char* right);

    friend bool operator!=(const char* left, const String& right);

    friend bool operator>(const String& left, const String& right);

    friend bool operator>(const String& left, const char* right);

    friend bool operator>(const char* left, const String& right);

    friend bool operator>=(const String& left, const String& right);

    friend bool operator>=(const String& left, const char* right);

    friend bool operator>=(const char* left, const String& right);

    friend bool operator<(const String& left, const String& right);

    friend bool operator<(const String& left, const char* right);

    friend bool operator<(const char* left, const String& right);

    friend bool operator<=(const String& left, const String& right);

    friend bool operator<=(const String& left, const char* right);

    friend bool operator<=(const char* left, const String& right);

    friend std::ostream& operator<<(std::ostream& os, const String& str);
public:
    static String FromStdString(const std::string& str);

    static String FromCString(const char* str);

    static String FromInt8(int8 value);

    static String FromInt16(int16 value);

    static String FromInt32(int32 value);

    static String FromInt64(int64 value);

    static String FromUInt8(uint8 value);

    static String FromUInt16(uint16 value);

    static String FromUInt32(uint32 value);

    static String FromUInt64(uint64 value);

    static String FromFloat(float value);

    static String FromDouble(double value);

    static String FromBool(bool value);
public:
    // 空字符串
    inline static const char* Empty = "";
private:
    /// <summary>
    /// 获取UTF-8字符的字节长度（从首字节判断）
    /// </summary>
    int32 _GetCharLength(unsigned char firstByte) const;
    /// <summary>
    /// 验证是否为有效的UTF-8连续字节
    /// </summary>
    bool _IsValidTrailByte(unsigned char byte) const;
    /// <summary>
    /// 计算 UTF-8 字符串的字符数量
    /// </summary>
    int64 _CalcCharCount(const char* str) const;
    /// <summary>
    /// 计算 UTF-8 字符串的字符数量（std::string版本）
    /// </summary>
    int64 _CalcCharCount(const std::string& str) const;
    /// <summary>
    /// 计算从开始到指定字节位置的字符数
    /// </summary>
    int64 _CalcCharCount(const char* str, int64 bytePos) const;
    /// <summary>
    /// 计算从开始到指定字节位置的字符数
    /// </summary>
    int64 _CalcCharCount(const std::string& str, int64 bytePos) const;
    /// <summary>
    /// 将字符索引转换为字节位置（优化版）
    /// </summary>
    int64 _IndexToPos(const std::string& str, int64 index) const;
    /// <summary>
    /// 从字节位置获取完整的UTF-8字符（新功能）
    /// </summary>
    std::string _GetCharAtBytePos(const std::string& str, int64 bytePos) const;
    /// <summary>
    /// 验证整个字符串是否为有效的UTF-8编码（新功能）
    /// </summary>
    bool _IsValidUTF8String(const std::string& str) const;
    /// <summary>
    /// 获取下一个字符的起始位置（新功能）
    /// </summary>
    int64 _NextCharPos(const std::string& str, int64 currentPos) const;
    /// <summary>
    /// 获取上一个字符的起始位置（新功能）
    /// </summary>
    int64 _PrevCharPos(const std::string& str, int64 currentPos) const;
private:
    // UTF-8 字符串
    std::string m_data;
    // Unicode 字符数量
    int64 m_count;
};