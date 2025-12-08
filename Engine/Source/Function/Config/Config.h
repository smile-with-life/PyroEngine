#pragma once
#include "Core.h"
#include "String/String.h"

enum class ConfigType
{
    Int32,
    Int64,
    Float,
    Double,
    String,
    Bool,
    Null
};

class ConfigValue
{
public:
    ConfigValue() = default;

    ~ConfigValue() = default;

    ConfigValue(const ConfigValue& other) = default;

    ConfigValue& operator=(const ConfigValue& other) = default;

    ConfigValue(ConfigValue&& other) noexcept = default;

    ConfigValue& operator=(ConfigValue&& other) noexcept = default;

    ConfigValue(int32 value);

    ConfigValue(int64 value);

    ConfigValue(float value);

    ConfigValue(double value);

    ConfigValue(const String& value);

    ConfigValue(bool value);
public:
    int32 ToInt32() const;

    int64 ToInt64() const;

    float ToFloat() const;

    double ToDouble() const;

    String ToString() const;

    bool ToBool() const;
public:
    String m_value = String::Empty;

    ConfigType m_type = ConfigType::Null;
};

class ConfigItem
{
public:
    
};

class ConfigGroup
{

};

class Config
{

};