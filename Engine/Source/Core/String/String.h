#pragma once

#include "Core.h"
#include "Container/Array.h"
#include "String/Char.h"
#include "Memory/ByteArray.h"

#include <filesystem>
#include <format>


class String;
using StringList = Array<String>;

/// <summary>
/// UTF-8 编码字符串
/// </summary>
class String
{
public:
    friend class Char;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    String() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~String() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    String(const String& other);
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    String& operator=(const String& other);
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    String(String&& other);
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    String& operator=(String&& other);
    /// <summary>
    /// 构造函数，从 C 风格字符串构造 String 对象
    /// </summary>
    /// <param name="str">以空字符结尾的 UTF-8 字符串</param>
    String(const char* str);
    /// <summary>
    /// 从 C 风格字符串赋值给当前 String 对象
    /// </summary>
    /// <param name="str">以空字符结尾的 UTF-8 字符串</param>
    /// <returns>this</returns>
    String& operator=(const char* str);
    /// <summary>
    /// 构造函数，从 std::string 对象构造 String 对象
    /// </summary>
    /// <param name="str">包含 UTF-8 数据的 std::string 对象</param>
    String(const std::string& str);
    /// <summary>
    /// 从 std::string 对象赋值给当前 String 对象
    /// </summary>
    /// <param name="str">包含 UTF-8 数据的 std::string 对象</param>
    /// <returns>this</returns>
    String& operator=(const std::string& str);
    /// <summary>
    /// 构造函数，从字节数组构造 String 对象
    /// </summary>
    /// <param name="buffer">包含 UTF-8 数据的字节数组</param>
    String(const ByteArray& buffer);
    /// <summary>
    /// 从字节数组赋值给当前 String 对象
    /// </summary>
    /// <param name="buffer">包含 UTF-8 数据的字节数组</param>
    /// <returns>this</returns>
    String& operator=(const ByteArray& buffer);
public:
    /// <summary>
    /// 返回字符串当前预分配的内存空间大小（字节数）
    /// </summary>
    /// <returns>预分配空间大小（字节）</returns>
    int64 Capacity() const;
    /// <summary>
    /// 返回字符串的字节大小（UTF-8 字节数）
    /// </summary>
    /// <returns>字节大小</returns>
    int64 Size() const;
    /// <summary>
    /// 返回字符串的 Unicode 字符数量
    /// </summary>
    /// <returns>字符数量</returns>
    int64 Count() const;
    /// <summary>
    /// 将字符串附加到当前字符串的末尾
    /// </summary>
    /// <param name="str">要附加的字符串</param>
    /// <returns>this</returns>
    String& Append(const String& str);
    /// <summary>
    /// 将字符附加到当前字符串的末尾
    /// </summary>
    /// <param name="ch">要附加的字符</param>
    /// <returns>this</returns>
    String& Append(const Char& ch);
    /// <summary>
    /// 将字符串添加到当前字符串的开头
    /// </summary>
    /// <param name="str">要添加的字符串</param>
    /// <returns>this</returns>
    String& Prepend(const String& str);
    /// <summary>
    /// 将字符添加到当前字符串的开头
    /// </summary>
    /// <param name="ch">要添加的字符</param>
    /// <returns>this</returns>
    String& Prepend(const Char& ch);
    /// <summary>
    /// 比较当前字符串与另一个字符串
    /// </summary>
    /// <param name="str">要比较的字符串</param>
    /// <returns>如果当前字符串小于 str 返回负数，等于返回 0，大于返回正数</returns>
    int64 Compare(const String& str) const;
    /// <summary>
    /// 判断字符串是否包含指定的子串
    /// </summary>
    /// <param name="str">要查找的子串</param>
    /// <returns>如果包含返回 true，否则返回 false</returns>
    bool Contains(const String& str) const;
    /// <summary>
    /// 判断字符串是否包含指定的字符
    /// </summary>
    /// <param name="ch">要查找的字符</param>
    /// <returns>如果包含返回 true，否则返回 false</returns>
    bool Contains(const Char& ch) const;
    /// <summary>
    /// 返回指定子串在字符串中首次出现的索引位置
    /// </summary>
    /// <param name="str">要查找的子串</param>
    /// <returns>子串首次出现的字符索引，如果未找到返回 -1</returns>
    int64 IndexOf(const String& str) const;
    /// <summary>
    /// 返回指定字符在字符串中首次出现的索引位置
    /// </summary>
    /// <param name="ch">要查找的字符</param>
    /// <returns>字符首次出现的字符索引，如果未找到返回 -1</returns>
    int64 IndexOf(const Char& ch) const;
    /// <summary>
    /// 返回指定子串在字符串中最后出现的索引位置
    /// </summary>
    /// <param name="str">要查找的子串</param>
    /// <returns>子串最后出现的字符索引，如果未找到返回 -1</returns>
    int64 LastIndexOf(const String& str) const;
    /// <summary>
    /// 返回指定字符在字符串中最后出现的索引位置
    /// </summary>
    /// <param name="ch">要查找的字符</param>
    /// <returns>字符最后出现的字符索引，如果未找到返回 -1</returns>
    int64 LastIndexOf(const Char& ch) const;
    /// <summary>
    /// 判断字符串是否以指定的前缀开头
    /// </summary>
    /// <param name="str">前缀字符串</param>
    /// <returns>如果以该前缀开头返回 true，否则返回 false</returns>
    bool StartWith(const String& str) const;
    /// <summary>
    /// 判断字符串是否以指定的后缀结尾
    /// </summary>
    /// <param name="str">后缀字符串</param>
    /// <returns>如果以该后缀结尾返回 true，否则返回 false</returns>
    bool EndWith(const String& str) const;
    /// <summary>
    /// 从字符串起始位置移除指定数量的字符
    /// </summary>
    /// <param name="count">要移除的字符数量</param>
    /// <returns>this</returns>
    String& RemoveLeft(int64 count);
    /// <summary>
    /// 从字符串中间指定位置开始移除指定数量的字符
    /// </summary>
    /// <param name="index">起始字符索引</param>
    /// <param name="count">要移除的字符数量</param>
    /// <returns>this</returns>
    String& Remove(int64 index, int64 count);
    /// <summary>
    /// 从字符串末尾移除指定数量的字符
    /// </summary>
    /// <param name="count">要移除的字符数量</param>
    /// <returns>this</returns>
    String& RemoveRight(int64 count);
    /// <summary>
    /// 保留字符串最左侧的 count 个字符，其余部分舍弃
    /// </summary>
    /// <param name="count">要保留的字符数量</param>
    /// <returns>this</returns>
    String& RemianLeft(int64 count);
    /// <summary>
    /// 保留字符串从 index 位置开始的 count 个字符，其余部分舍弃
    /// </summary>
    /// <param name="index">起始字符索引</param>
    /// <param name="count">要保留的字符数量</param>
    /// <returns>this</returns>
    String& Remian(int64 index, int64 count);
    /// <summary>
    /// 保留字符串最右侧的 count 个字符，其余部分舍弃
    /// </summary>
    /// <param name="count">要保留的字符数量</param>
    /// <returns>this</returns>
    String& RemianRight(int64 count);
    /// <summary>
    /// 返回一个字符串，包含原字符串最左侧的 count 个字符
    /// </summary>
    /// <param name="count">要获取的字符数量</param>
    /// <returns>包含左侧 count 个字符的新字符串</returns>
    String LeftStr(int64 count);
    /// <summary>
    /// 返回一个字符串，包含原字符串从 index 位置开始的 count 个字符
    /// </summary>
    /// <param name="index">起始字符索引</param>
    /// <param name="count">要获取的字符数量</param>
    /// <returns>包含指定范围字符的新字符串</returns>
    String SubStr(int64 index, int64 count);
    /// <summary>
    /// 返回一个字符串，包含原字符串最右侧的 count 个字符
    /// </summary>
    /// <param name="count">要获取的字符数量</param>
    /// <returns>包含右侧 count 个字符的新字符串</returns>
    String RightStr(int64 count);
    /// <summary>
    /// 移除字符串左侧的空白字符
    /// </summary>
    /// <returns>this</returns>
    String& TrimLeft();
    /// <summary>
    /// 移除字符串左右两侧的空白字符（空格、制表符、换行符等）
    /// </summary>
    /// <returns>this</returns>
    String& Trim();
    /// <summary>
    /// 移除字符串右侧的空白字符
    /// </summary>
    /// <returns>this</returns>
    String& TrimRight();
    /// <summary>
    /// 使用指定的分隔符分割字符串，返回各部分组成的列表
    /// </summary>
    /// <param name="sep">分隔符字符</param>
    /// <returns>分割后的字符串列表</returns>
    StringList Split(char sep) const;
    /// <summary>
    /// 使用正则表达式匹配字符串，返回第一个匹配的子串
    /// </summary>
    /// <param name="regex">正则表达式字符串</param>
    /// <returns>匹配到的子串</returns>
    String Match(const String& regex);
    /// <summary>
    /// 在指定位置插入字符串
    /// </summary>
    /// <param name="index">插入位置的字符索引</param>
    /// <param name="str">要插入的字符串</param>
    /// <returns>this</returns>
    String& Insert(int64 index, const String& str);
    // <summary>
    /// 替换字符串的指定部分
    /// </summary>
    /// <param name="index">起始字符索引</param>
    /// <param name="count">要替换的字符数量</param>
    /// <param name="str">用于替换的字符串</param>
    /// <returns>this</returns>
    String& Replace(int64 index, int64 count, const String& str);
    /// <summary>
    /// 将当前字符串重复指定的次数
    /// </summary>
    /// <param name="count">重复次数</param>
    /// <returns>this</returns>
    String& Repeat(int64 count);
    /// <summary>
    /// 清空字符串内容
    /// </summary>
    void Clear();
    /// <summary>
    /// 预分配至少指定大小的内存空间（字节）
    /// </summary>
    /// <param name="size">要预分配的字节数</param>
    void Reserve(int64 size);
    /// <summary>
    /// 释放未使用的内存容量，使容量等于当前字符串的大小
    /// </summary>
    void Shrink();
    /// <summary>
    /// 判断字符串是否为空
    /// </summary>
    /// <returns>如果为空返回 true，否则返回 false</returns>
    bool IsEmpty() const;
    /// <summary>
    /// 判断字符串是否仅包含数字字符
    /// </summary>
    /// <returns>如果仅包含数字字符返回 true，否则返回 false</returns>
    bool IsNumeric() const;
    /// <summary>
    /// 判断指定的索引是否为有效的字符索引
    /// </summary>
    /// <param name="index">要检查的字符索引</param>
    /// <returns>如果索引有效返回 true，否则返回 false</returns>
    bool IsValidIndex(int64 index) const;
    /// <summary>
    /// 交换当前字符串与另一个字符串的内容
    /// </summary>
    /// <param name="other">要交换的另一个字符串</param>
    void Swap(String& other);
    /// <summary>
    /// 将字符串转换为布尔值
    /// </summary>
    /// <returns>转换后的布尔值</returns>
    bool ToBool() const;
    /// <summary>
    /// 将字符串转换为 int8 类型的整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 int8 值</returns>
    int8 ToInt8(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 int16 类型的整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 int16 值</returns>
    int16 ToInt16(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 int32 类型的整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 int32 值</returns>
    int32 ToInt32(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 int64 类型的整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 int64 值</returns>
    int64 ToInt64(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 uint8 类型的无符号整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 uint8 值</returns>
    uint8 ToUInt8(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 uint16 类型的无符号整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 uint16 值</returns>
    uint16 ToUInt16(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 uint32 类型的无符号整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 uint32 值</returns>
    uint32 ToUInt32(int base = 10) const;
    /// <summary>
    /// 将字符串转换为 uint64 类型的无符号整数值
    /// </summary>
    /// <param name="base">进制基数（默认 10，支持 2-16）</param>
    /// <returns>转换后的 uint64 值</returns>
    uint64 ToUInt64(int base = 10) const;
    /// <summary>
    /// 将字符串转换为单精度浮点数
    /// </summary>
    /// <returns>转换后的 float 值</returns>
    float ToFloat() const;
    /// <summary>
    /// 将字符串转换为双精度浮点数
    /// </summary>
    /// <returns>转换后的 double 值</returns>
    double ToDouble() const;
    /// <summary>
    /// 将当前 String 对象转换为 std::string 对象
    /// </summary>
    /// <returns>包含相同 UTF-8 数据的 std::string 对象</returns>
    std::string ToStdString() const;
    /// <summary>
    /// 将当前 String 对象转换为以空字符结尾的 C 风格字符串
    /// </summary>
    /// <returns>指向内部 UTF-8 数据的 const char* 指针</returns>
    const char* ToCString() const;
public:
    /// <summary>
    /// 隐式转换为 std::filesystem::path 对象，便于文件系统操作
    /// </summary>
    operator std::filesystem::path() const;
    /// <summary>
    /// 隐式转换为 const std::string 对象
    /// </summary>
    operator const std::string() const;
    /// <summary>
    /// 隐式转换为 bool 值，判断字符串是否非空
    /// </summary>
    operator bool() const;
    /// <summary>
    /// 将 String 对象输出到输出流
    /// </summary>
    friend std::ostream& operator<<(std::ostream& os, const String& str);
    /// <summary>
    /// 获取指定字符索引处的字符
    /// </summary>
    Char operator[](int64 index);
    /// <summary>
    /// 加法运算符
    /// </summary>
    friend String operator+(const String& left, const String& right);
    /// <summary>
    /// 加法运算符
    /// </summary>
    friend String operator+(const String& left, const char* right);
    /// <summary>
    /// 加法运算符
    /// </summary>
    friend String operator+(const char* left, const String& right);
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    String& operator+=(const String& str);
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    String& operator+=(Char ch);
    /// <summary>
    /// 相等运算符
    /// </summary>
    friend bool operator==(const String& left, const String& right);
    /// <summary>
    /// 相等运算符
    /// </summary>
    friend bool operator==(const String& left, const char* right);
    /// <summary>
    /// 相等运算符
    /// </summary>
    friend bool operator==(const char* left, const String& right);
    /// <summary>
    /// 不等运算符
    /// </summary>
    friend bool operator!=(const String& left, const String& right);
    /// <summary>
    /// 不等运算符
    /// </summary>
    friend bool operator!=(const String& left, const char* right);
    /// <summary>
    /// 不等运算符
    /// </summary>
    friend bool operator!=(const char* left, const String& right);
    /// <summary>
    /// 小于运算符
    /// </summary>
    friend bool operator<(const String& left, const String& right);
    /// <summary>
    /// 小于运算符
    /// </summary>
    friend bool operator<(const String& left, const char* right);
    /// <summary>
    /// 小于运算符
    /// </summary>
    friend bool operator<(const char* left, const String& right);
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    friend bool operator<=(const String& left, const String& right);
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    friend bool operator<=(const String& left, const char* right);
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    friend bool operator<=(const char* left, const String& right);
    /// <summary>
    /// 大于运算符
    /// </summary>
    friend bool operator>(const String& left, const String& right);
    /// <summary>
    /// 大于运算符
    /// </summary>
    friend bool operator>(const String& left, const char* right);
    /// <summary>
    /// 大于运算符
    /// </summary>
    friend bool operator>(const char* left, const String& right);
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    friend bool operator>=(const String& left, const String& right);
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    friend bool operator>=(const String& left, const char* right);
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    friend bool operator>=(const char* left, const String& right);
public:
    /// <summary>
    /// 从 std::string 对象创建 String 对象（静态工厂方法）。
    /// </summary>
    /// <param name="str">包含 UTF-8 数据的 std::string 对象</param>
    /// <returns>新创建的 String 对象</returns>
    static String FromStdString(const std::string& str);
    /// <summary>
    /// 从 C 风格字符串创建 String 对象（静态工厂方法）。
    /// </summary>
    /// <param name="str">以空字符结尾的 UTF-8 字符串</param>
    /// <returns>新创建的 String 对象</returns>
    static String FromCString(const char* str);
    /// <summary>
    /// 从 int8 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 int8 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromInt8(int8 value);
    /// <summary>
    /// 从 int16 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 int16 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromInt16(int16 value);
    /// <summary>
    /// 从 int32 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 int32 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromInt32(int32 value);
    /// <summary>
    /// 从 int64 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 int64 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromInt64(int64 value);
    /// <summary>
    /// 从 uint8 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 uint8 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromUInt8(uint8 value);
    /// <summary>
    /// 从 uint16 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 uint16 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromUInt16(uint16 value);
    /// <summary>
    /// 从 uint32 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 uint32 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromUInt32(uint32 value);
    /// <summary>
    /// 从 uint64 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 uint64 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromUInt64(uint64 value);
    /// <summary>
    /// 从 float 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 float 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromFloat(float value);
    /// <summary>
    /// 从 double 值创建 String 对象。
    /// </summary>
    /// <param name="value">要转换的 double 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromDouble(double value);
    /// <summary>
    /// 从 bool 值创建 String 对象。
    /// 返回 "true" 或 "false"。
    /// </summary>
    /// <param name="value">要转换的 bool 值</param>
    /// <returns>转换后的 String 对象</returns>
    static String FromBool(bool value);
public:
    /// <summary>
    /// 空字符串
    /// </summary>
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
    /// 将字符索引转换为字节位置
    /// </summary>
    int64 _IndexToPos(const std::string& str, int64 index) const;
    /// <summary>
    /// 从字节位置获取完整的UTF-8字符
    /// </summary>
    std::string _GetCharAtBytePos(const std::string& str, int64 bytePos) const;
    /// <summary>
    /// 验证整个字符串是否为有效的UTF-8编码
    /// </summary>
    bool _IsValidUTF8String(const std::string& str) const;
    /// <summary>
    /// 获取下一个字符的起始位置
    /// </summary>
    int64 _NextCharPos(const std::string& str, int64 currentPos) const;
    /// <summary>
    /// 获取上一个字符的起始位置
    /// </summary>
    int64 _PrevCharPos(const std::string& str, int64 currentPos) const;
private:
    // UTF-8 字符串
    std::string m_data;
    // Unicode 字符数量
    int64 m_count;
};

using StringView = std::string_view;

/// <summary>
/// 为 String 提供 format 格式化支持
/// </summary>
template<>
struct std::formatter<String> 
{
    constexpr auto parse(std::format_parse_context& ctx) 
    {
        return ctx.begin(); 
    }

    auto format(const String& str, std::format_context& ctx) const 
    {
        return std::format_to(ctx.out(), "{}", str.ToStdString());
    }
};