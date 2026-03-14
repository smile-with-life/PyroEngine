#pragma once

#include "Core.h"
#include "String/String.h"
#include "Json/Json.h"

using ConfigKey = String;

class Config
{
public:
    using Member = std::string;
    using ArrayIndex = int64;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Config() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Config() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    Config(const Config& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    Config& operator=(const Config& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    Config(Config&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    Config& operator=(Config&& other) = default;
    /// <summary>
    /// 构造函数，从 Json 构造
    /// </summary>
    /// <param name="value"></param>
    explicit Config(const Json& json);
public:
    bool GetValue(const String& expression, int32& value);

    bool GetValue(const String& expression, int64& value);

    bool GetValue(const String& expression, float& value);

    bool GetValue(const String& expression, double& value);

    bool GetValue(const String& expression, bool& value);

    bool GetValue(const String& expression, String& value);

    void GetValueOrDefault(const String& expression, int32& value, int32 defaultValue);

    void GetValueOrDefault(const String& expression, int64& value, int64 defaultValue);

    void GetValueOrDefault(const String& expression, float& value, float defaultValue);

    void GetValueOrDefault(const String& expression, double& value, double defaultValue);

    void GetValueOrDefault(const String& expression, bool& value, bool defaultValue);

    void GetValueOrDefault(const String& expression, String& value, String defaultValue);
public:
    bool IsValid() const;
private:
    bool _ParseExpression(const String& expression);

    const Json* _Navigate() const;
private:
    /// <summary>
    /// 配置值
    /// </summary>
    Json m_value;
    /// <summary>
    /// 步骤序列
    /// </summary>
    std::vector<std::variant<Member, ArrayIndex>> m_steps;
};

