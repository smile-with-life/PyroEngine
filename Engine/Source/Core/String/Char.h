#pragma once

#include "Core.h"
#include "String/String.h"

class Char 
{
public:
    friend class String;
public:
    // 默认构造函数
    Char() = default;

    // 析构函数
    ~Char() = default;

    // 拷贝构造函数
    Char(const Char& other);

    // 拷贝赋值
    Char& operator=(const Char& other);

    // 移动构造函数
    Char(Char&& other);

    // 移动赋值
    Char& operator=(Char&& other);

    explicit Char(char c);

    explicit Char(const std::string& utf8Char);

    explicit Char(uint32 unicode);

    // 隐式转换
    operator bool() const;
public:
    // 返回字符的 Unicode 值
    uint32 Unicode() const;

    // 是否为有效的 UTF-8 编码的字符
    bool IsValid() const;

    // 判断是否为小写字符
    bool IsLower() const;

    // 判断是否为大写字符
    bool IsUpper() const;

    // 判断是否为空字符
    bool IsNull() const;

    //  判断是否为数字字符
    bool IsNumber() const;

    // 判断是否为可打印字符
    bool IsPrint() const;

    // 判断是否为空白字符
    bool IsSpace() const;

    Char ToLower() const;

    Char ToUpper() const;
private:
    bool _IsValidUTF8(std::string str) const
    {
        if (str.empty()) return false;
        const unsigned char* data = reinterpret_cast<const unsigned char*>(str.data());
        size_t len = str.size();

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
private:
    std::string m_data;
};