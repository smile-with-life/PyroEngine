#pragma once

#include "Core.h"
#include "Container/Array.h"
#include "Container/Map.h"
#include "String/String.h"

#include <variant>

class Json;
using JsonNull = std::nullptr_t;
using JsonBool = bool;
using JsonInt = int32;
using JsonFloat = double;
using JsonString = String;
using JsonArray = Array<Json>;
using JsonObject = Map<String, Json>;

/// <summary>
/// Json 数据类型
/// </summary>
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

/// <summary>
/// Json 对象
/// </summary>
class Json
{
public:
    Json() noexcept = default;

    Json(const Json& other) = default;

    Json& operator=(const Json& other) = default;

    Json(Json&& other) = default;

    Json& operator=(Json&& other) = default;

    Json(std::nullptr_t) noexcept;

    Json& operator=(std::nullptr_t) noexcept;

    Json(JsonBool value);

    Json& operator=(JsonBool value) noexcept;

    Json(JsonInt value);

    Json& operator=(JsonInt value) noexcept;

    Json(JsonFloat value);

    Json& operator=(JsonFloat value) noexcept;

    Json(const JsonString& value);

    Json& operator=(const JsonString& value) noexcept;

    Json(const char* value);

    Json& operator=(const char* value) noexcept;

    Json(const JsonArray& value);

    Json& operator=(const JsonArray& value) noexcept;

    Json(const JsonObject& value); 

    Json& operator=(const JsonObject& value) noexcept;

    Json(JsonType type);
public:
    String Dump() const;

    JsonType Type() const;

    JsonBool AsBool() const;

    JsonInt AsInt() const;

    JsonFloat AsFloat() const;

    JsonString AsString() const;

    JsonArray AsArray() const;

    JsonObject AsObject() const;

    bool IsNull() const noexcept;

    bool IsBool() const noexcept;

    bool IsInt() const noexcept;

    bool IsFloat() const noexcept;

    bool IsString() const noexcept;

    bool IsArray() const noexcept;

    bool IsObject() const noexcept;

    void Swap(Json& other) noexcept;
public:
    operator bool() const;
    // 控制台打印操作
    friend std::ostream& operator<<(std::ostream& os, const Json& json);

    // 数组访问操作
    Json& operator[](int32 index);

    // 数组访问操作 const
    const Json& operator[](int32 index) const;

    // 对象访问操作
    Json& operator[](const String& key);

    const Json& operator[](const String& key) const;

    Json& operator[](const char* key);

    const Json& operator[](const char* key) const;

    friend bool operator==(const Json& left, const Json& right);

    friend bool operator!=(const Json& left, const Json& right);

    friend bool operator==(const Json& left, std::nullptr_t);

    friend bool operator!=(const Json& left, std::nullptr_t);

    friend bool operator==(std::nullptr_t, const Json& right);

    friend bool operator!=(std::nullptr_t, const Json& right);
public:
    static Json Parse(const String& jsonString);

    static Json Array();

    static Json Object();
private:
    template<class Type>
    bool _CheckJsonType() const
    {
        if constexpr (std::is_same_v<Type, JsonBool>) return m_type == JsonType::Bool;
        if constexpr (std::is_same_v<Type, JsonInt>) return m_type == JsonType::Int;
        if constexpr (std::is_same_v<Type, JsonFloat>) return m_type == JsonType::Float;
        if constexpr (std::is_same_v<Type, JsonString>) return m_type == JsonType::String;
        if constexpr (std::is_same_v<Type, JsonArray>) return m_type == JsonType::Array;
        if constexpr (std::is_same_v<Type, JsonObject>) return m_type == JsonType::Object;
        return m_type == JsonType::Null;
    }

    template<class Type>
    const Type& _GetValue() const
    {
        if (!_CheckJsonType<Type>())
        {
            throw std::bad_variant_access();
        }
        return std::get<Type>(m_data);
    }

    template<class Type>
    Type& _GetValue()
    {
        return const_cast<Type&>(static_cast<const Json*>(this)->_GetValue<Type>());
    }

    String _EscapeChar(const String& str) const;

    void _Serialize(std::ostream& os, int indentLevel, int indentSize) const;
private:
    /// <summary>
    /// Json 数据类型
    /// </summary>
    JsonType m_type = JsonType::Null;
    /// <summary>
    /// Json 数据，使用 std::variant 存储不同类型的数据
    /// </summary>
    std::variant<std::monostate, JsonBool, JsonInt, JsonFloat, JsonString, JsonArray, JsonObject> m_data;
};


// 解析器类
class JsonParser
{
public:
    JsonParser(const String& input)
        : m_input(input.ToCString())
    {

    }

    Json Parse()
    {
        Json result = _ParseValue();
        _SkipWhitespace();
        if (m_pos < m_input.size())
        {
            throw std::runtime_error("Unexpected trailing characters");
        }
        return result;
    }
private:
    void _SkipWhitespace()
    {
        while (m_pos < m_input.size() &&
            (m_input[m_pos] == ' ' || m_input[m_pos] == '\n' ||
             m_input[m_pos] == '\r' || m_input[m_pos] == '\t'))
        {
            ++m_pos;
        }
    }

    char _Peek() const
    {
        return m_pos < m_input.size() ? m_input[m_pos] : '\0';
    }

    char _Get()
    {
        return m_pos < m_input.size() ? m_input[m_pos++] : '\0';
    }

    bool _Match(char expected)
    {
        if (_Peek() == expected)
        {
            ++m_pos;
            return true;
        }
        return false;
    }

    void _Expect(char expected)
    {
        if (!_Match(expected))
        {
            throw std::runtime_error(std::format("Expected {} at position {}", expected, m_pos));
        }
    }

    String _ParseString()
    {
        _Expect('"');
        String result;

        while (m_pos < m_input.size() && _Peek() != '"')
        {
            if (_Peek() == '\\')
            {
                ++m_pos; // 跳过反斜杠
                char ch = _Get();
                switch (ch)
                {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default:
                    throw std::runtime_error("Invalid escape sequence");
                }
            }
            else
            {
                result += _Get();
            }
        }
        _Expect('"');
        return result;
    }

    Json _ParseNumber()
    {
        int32 start = m_pos;

        // 处理负号
        if (_Peek() == '-')
        {
            ++m_pos;
        }

        // 处理整数部分
        if (_Peek() == '0')
        {
            ++m_pos;
        }
        else if (std::isdigit(_Peek()))
        {
            while (m_pos < m_input.size() && std::isdigit(_Peek()))
            {
                ++m_pos;
            }
        }

        bool isFloat = false;

        // 处理小数部分
        if (_Match('.'))
        {
            isFloat = true;
            if (!std::isdigit(_Peek()))
            {
                throw std::runtime_error("Expected digits after decimal point");
            }
            while (m_pos < m_input.size() && std::isdigit(_Peek()))
            {
                ++m_pos;
            }
        }

        // 处理指数部分
        if (_Match('e') || _Match('E'))
        {
            isFloat = true;
            if (_Match('+') || _Match('-')) {}
            if (!std::isdigit(_Peek()))
            {
                throw std::runtime_error("Expected digits in exponent");
            }
            while (m_pos < m_input.size() && std::isdigit(_Peek()))
            {
                ++m_pos;
            }
        }

        StringView numStr = m_input.substr(start, m_pos - start);

        if (isFloat)
        {
            return Json(std::stod(numStr.data()));
        }
        else
        {
            return Json(static_cast<JsonInt>(std::stoll(numStr.data())));
        }
    }

    Json _ParseArray()
    {
        _Expect('[');
        _SkipWhitespace();

        JsonArray result;

        if (_Peek() == ']')
        {
            ++m_pos;
            return Json(result);
        }

        while (true)
        {
            result.Add(_ParseValue());
            _SkipWhitespace();

            if (_Match(']'))
                break;
            _Expect(',');
            _SkipWhitespace();
        }

        return Json(result);
    }

    Json _ParseObject()
    {
        _Expect('{');
        _SkipWhitespace();

        JsonObject result;

        if (_Peek() == '}')
        {
            ++m_pos;
            return Json(result);
        }

        while (true)
        {
            if (_Peek() != '"')
            {
                throw std::runtime_error("Expected string key");
            }

            String key = _ParseString();
            _SkipWhitespace();
            _Expect(':');
            _SkipWhitespace();

            result.Add({ key, _ParseValue() });
            _SkipWhitespace();

            if (_Match('}'))
                break;
            _Expect(',');
            _SkipWhitespace();
        }

        return Json(result);
    }

    Json _ParseValue()
    {
        _SkipWhitespace();

        switch (_Peek())
        {
        case 'n':
            if (m_input.substr(m_pos, 4) == "null")
            {
                m_pos += 4;
                return Json(nullptr);
            }
            break;
        case 't':
            if (m_input.substr(m_pos, 4) == "true")
            {
                m_pos += 4;
                return Json(true);
            }
            break;
        case 'f':
            if (m_input.substr(m_pos, 5) == "false")
            {
                m_pos += 5;
                return Json(false);
            }
            break;
        case '"':
            return Json(_ParseString());
        case '[':
            return _ParseArray();
        case '{':
            return _ParseObject();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return _ParseNumber();
        default:
            throw std::runtime_error(std::format("Unexpected character {} at position {}", _Peek(), m_pos));
        }

        throw std::runtime_error("Invalid JSON value");
    }
private:
    StringView m_input;
    int32 m_pos = 0;
};