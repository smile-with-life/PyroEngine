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
    bool GetValue(const String& expression, int32& value) const;

    bool GetValue(const String& expression, int64& value) const;

    bool GetValue(const String& expression, float& value) const;

    bool GetValue(const String& expression, double& value) const;

    bool GetValue(const String& expression, bool& value) const;

    bool GetValue(const String& expression, String& value) const;

    bool GetValue(const String& expression, Config& value) const;

    void GetValueOrDefault(const String& expression, int32& value, int32 defaultValue) const;

    void GetValueOrDefault(const String& expression, int64& value, int64 defaultValue) const;

    void GetValueOrDefault(const String& expression, float& value, float defaultValue) const;

    void GetValueOrDefault(const String& expression, double& value, double defaultValue) const;

    void GetValueOrDefault(const String& expression, bool& value, bool defaultValue) const;

    void GetValueOrDefault(const String& expression, String& value, String defaultValue) const;

    void GetValueOrDefault(const String& expression, Config& value, Config defaultValue) const;

    void GetArray(const String& expression, Array<int32>& array) const;

    void GetArray(const String& expression, Array<int64>& array) const;

    void GetArray(const String& expression, Array<float>& array) const;

    void GetArray(const String& expression, Array<double>& array) const;

    void GetArray(const String& expression, Array<bool>& array) const;

    void GetArray(const String& expression, Array<String>& array) const;

    void GetArray(const String& expression, Array<Config>& array) const;
public:
    bool IsValid() const;
private:
    bool _ParseExpression(const String& expression) const;

    const Json* _Navigate() const;
private:
    /// <summary>
    /// 配置值
    /// </summary>
    Json m_value;
    /// <summary>
    /// 步骤序列
    /// </summary>
    mutable std::vector<std::variant<Member, ArrayIndex>> m_steps;
};

