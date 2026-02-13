#pragma once

#include "Core.h"

#include <string>

class Char
{
public:
    friend class String;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Char() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Char() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    Char(const Char& other);
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    Char& operator=(const Char& other);
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    Char(Char&& other);
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    Char& operator=(Char&& other);
    /// <summary>
    /// 构造函数，从 Unicode 码点构造字符
    /// </summary>
    /// <param name="unicode">Unicode 码点值，范围 0x0000 - 0x10FFFF</param>
    explicit Char(uint32 unicode);
    /// <summary>
    /// 构造函数，从 ASCII 字符构造
    /// </summary>
    /// <param name="ch">ASCII 字符（0x00 - 0x7F）</param>
    explicit Char(char ch);
    /// <summary>
    /// 构造函数，从 UTF-8 编码的字符构造
    /// </summary>
    /// <param name="utf8Char">UTF-8 编码的单个字符，长度应为 1-4 字节</param>
    explicit Char(const std::string& utf8Char);
public:
    /// <summary>
    /// 返回字符的 Unicode 码点值
    /// </summary>
    /// <returns>Unicode 码点值，无效字符返回 0</returns>
    uint32 Unicode() const;
    /// <summary>
    /// 返回UTF-8字节数
    /// </summary>
    int32 Size() const;
    
    /// <summary>
    /// 是否为 ASCII 字符
    /// </summary>
    bool IsASCII() const;
    /// <summary>
    /// 判断字符是否为数字字符
    /// </summary>
    /// <returns>是数字字符返回 true，否则返回 false</returns>
    bool IsNumber() const;
    /// <summary>
    /// 是否为字母
    /// </summary>
    bool IsLetter() const;
    /// <summary>
    /// 是否为标点符号
    /// </summary>
    bool IsPunctuation() const;
    /// <summary>
    /// 是否为控制字符
    /// </summary>
    bool IsControl() const;
    /// <summary>
    /// 是否为CJK统一表意字符
    /// </summary>
    bool IsCJK() const;
    /// <summary>
    /// 是否为表情符号
    /// </summary>
    bool IsEmoji() const;
    /// <summary>
    /// 判断字符是否为空白字符
    /// </summary>
    /// <returns>是空白字符返回 true，否则返回 false</returns>
    bool IsSpace() const;
    /// <summary>
    /// 判断字符是否为可打印字符
    /// </summary>
    /// <returns>是可打印字符返回 true，否则返回 false</returns>
    bool IsPrint() const;
    /// <summary>
    /// 检查字符是否为有效的 UTF-8 编码字符
    /// </summary>
    /// <returns>有效返回 true，否则返回 false</returns>
    bool IsValid() const;
    /// <summary>
    /// 判断字符是否为空字符
    /// </summary>
    /// <returns>是空字符返回 true，否则返回 false</returns>
    bool IsNull() const;
    /// <summary>
    /// 判断字符是否为小写字母
    /// </summary>
    /// <returns>是小写字母返回 true，否则返回 false</returns>
    bool IsLower() const;
    /// <summary>
    /// 判断字符是否为大写字母
    /// </summary>
    /// <returns>是大写字母返回 true，否则返回 false</returns>
    bool IsUpper() const;
    /// <summary>
    /// 将字符转换为小写形式
    /// </summary>
    /// <returns>小写形式的字符对象</returns>
    Char ToLower() const;
    /// <summary>
    /// 将字符转换为大写形式
    /// </summary>
    /// <returns>大写形式的字符对象</returns>
    Char ToUpper() const; 
private:
    /// <summary>
    /// 验证UTF-8字符串是否为有效的单个字符
    /// </summary>
    bool _IsValidUTF8(const std::string& str) const;

    /// <summary>
    /// 验证Unicode码点的有效性
    /// </summary>
    bool _IsValidCodepoint(uint32 cp) const;

    /// <summary>
    /// 从UTF-8解码Unicode码点
    /// </summary>
    uint32 _DecodeUTF8(const std::string& str) const;

    /// <summary>
    /// 将Unicode码点编码为UTF-8
    /// </summary>
    std::string _EncodeUTF8(uint32 cp) const;
private:
    std::string m_data;
};