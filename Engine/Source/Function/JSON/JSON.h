#pragma once
#include "Core.h"
#include "Container/Array.h"
#include "Container/Map.h"
#include "String/String.h"
#include <variant>

// Json对象类型
enum class JsonType
{
    Null,
    Bool,
    Int,
    Float,
    String,
    Array,
    Object,
};

// Json对象
class Json
{
public:
    using string = String;
    using array = Array<Json>;
    using object = Map<String, Json>;
public:
    Json() noexcept = default;

    Json(const Json& other) = default;

    Json& operator=(const Json& other) = default;

    Json(Json&& other) = default;

    Json& operator=(Json&& other) = default;

    Json(std::nullptr_t) noexcept;

    Json(bool value);

    Json(int32 value);

    Json(double value);

    Json(const string& value);

    Json(const char* value);

    Json(const array& value);

    Json(const object& value);

    Json(JsonType type);
public:
    // 类型检查
    bool IsNull() const noexcept;

    bool IsBool() const noexcept;

    bool IsInt() const noexcept;

    bool IsFloat() const noexcept;

    bool IsString() const noexcept;

    bool IsArray() const noexcept;

    bool IsObject() const noexcept;

    // 类型转换
    template<class T>
    const T& Get() const
    {
        if (std::get_if<T>(&m_value) == nullptr)
        {
            std::cout << "?";
        }
        if (!CheckJsonType<T>()) [[unlikely]]
        {
            throw std::bad_cast();
        }
        return std::get<T>(m_value);
    }

    template<class T>
    T& Get()
    {
        return const_cast<T&>(static_cast<const Json*>(this)->Get<T>());
    }

    JsonType Type() const;

    void Append(const Json& json);

    bool Contains(const string& key) const;

    void Erase(const string& key);

    String Serialize() const;

    String ToString() const;
public:
    // 隐式转换 bool
    operator bool();

    // 隐式转换 int32
    operator int32();

    // 隐式转换 double
    operator double();

    // 隐式转换 string
    operator string();

    // 隐式转换 array
    operator array();

    // 隐式转换 object
    operator object();

    // 控制台打印操作
    friend std::ostream& operator<<(std::ostream& os, const Json& json);

    // 数组访问操作
    Json& operator[](int32 index);

    // 数组访问操作 const
    const Json& operator[](int32 index) const;

    // 对象访问操作
    Json& operator[](const string& key);

    // 对象访问操作 const
    const Json& operator[](const string& key) const;

    // 对象访问操作
    Json& operator[](const char* key);

    // 对象访问操作 const
    const Json& operator[](const char* key) const;

    Json& operator=(std::nullptr_t) noexcept;

    Json& operator=(bool value) noexcept;

    Json& operator=(int value) noexcept;

    Json& operator=(double value) noexcept;

    Json& operator=(const string& value) noexcept;

    Json& operator=(const array& value) noexcept;

    Json& operator=(const object& value) noexcept;

    friend bool operator==(const Json& left, const Json& right);

    friend bool operator!=(const Json& left, const Json& right);
public:
    // 解析
    static Json Parse(const string& jsonString);

    static Json Array();

    static Json Object();
private:
    template<typename T>
    bool CheckJsonType() const
    {
        if constexpr (std::is_same_v<T, bool>) return m_type == JsonType::Bool;
        if constexpr (std::is_same_v<T, int32>) return m_type == JsonType::Int;
        if constexpr (std::is_same_v<T, double>) return m_type == JsonType::Float;
        if constexpr (std::is_same_v<T, String>) return m_type == JsonType::String;
        if constexpr (std::is_same_v<T, array>) return m_type == JsonType::Array;
        if constexpr (std::is_same_v<T, object>) return m_type == JsonType::Object;
        return m_type == JsonType::Null;
    }

    void SerializeHelper(std::ostream& os, const Json& json) const;
private:
    static void SkipWhitespace(const char*& ptr);

    static String ParseString(const char*& ptr);

    static Json ParseNumber(const char*& ptr);

    static Json ParseValue(const char*& ptr);
private:
    JsonType m_type = JsonType::Null;

    std::variant<std::monostate, bool, int32, double, string, array, object> m_value;
};

using JsonArray = Array<Json>;
using JsonObject = Map<String, Json>;