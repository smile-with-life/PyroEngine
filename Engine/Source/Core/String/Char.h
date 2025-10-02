#pragma once

#include "Core.h"

class Char 
{
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
public:
    // 返回字符的 Unicode 值
    uint32 Unicode() const;

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
private:
    std::string m_data;
};