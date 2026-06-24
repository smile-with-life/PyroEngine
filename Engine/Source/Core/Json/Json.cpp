#include "pch.h"

#include "Json.h"

/* static */
Json Json::Parse(const String& jsonString)
{
    JsonParser parser(jsonString);
    return parser.Parse();
}

Json Json::Array()
{
    return Json(JsonType::Array);
}

Json Json::Object()
{
    return Json(JsonType::Object);
}


/* public */
Json::Json(std::nullptr_t) noexcept
    : m_type(JsonType::Null)
    , m_data(std::monostate{})
{

}

Json& Json::operator=(std::nullptr_t) noexcept
{
    m_type = JsonType::Null;
    m_data = std::monostate{};
    return *this;
}

Json::Json(JsonBool value)
    : m_type(JsonType::Bool)
    , m_data(value)
{

}

Json& Json::operator=(JsonBool value) noexcept
{
    m_type = JsonType::Bool;
    m_data = value;
    return *this;
}

Json::Json(JsonInt value)
    : m_type(JsonType::Int)
    , m_data(value)
{
    
}

Json& Json::operator=(JsonInt value) noexcept
{
    m_type = JsonType::Int;
    m_data = value;
    return *this;
}

Json::Json(JsonFloat value)
    : m_type(JsonType::Float)
    , m_data(value)
{

}

Json& Json::operator=(JsonFloat value) noexcept
{
    m_type = JsonType::Float;
    m_data = value;
    return *this;
}

Json::Json(const JsonString& value)
    : m_type(JsonType::String)
    , m_data(value)
{

}

Json& Json::operator=(const JsonString& value) noexcept
{
    m_type = JsonType::String;
    m_data = value;
    return *this;
}

Json::Json(const char* value)
    : m_type(JsonType::String)
    , m_data(JsonString(value))
{

}

Json& Json::operator=(const char* value) noexcept
{
    m_type = JsonType::String;
    m_data = JsonString(value);
    return *this;
}

Json::Json(const JsonArray& value)
    : m_type(JsonType::Array)
    , m_data(value)
{

}

Json& Json::operator=(const JsonArray& value) noexcept
{ 
    m_type = JsonType::Array;
    m_data = value;
    return *this;
}

Json::Json(const JsonObject& value)
    : m_type(JsonType::Object)
    , m_data(value)
{

}

Json& Json::operator=(const JsonObject& value) noexcept
{
    m_type = JsonType::Object;
    m_data = value;
    return *this;
}

Json::Json(JsonType type)
    : m_type(type)
{
    switch (type)
    {
    case JsonType::Null:
        m_data = std::monostate{};
        break;
    case JsonType::Bool:
        m_data = JsonBool(false);
        break;
    case JsonType::Int:
        m_data = JsonInt(0);
        break;
    case JsonType::Float:
        m_data = JsonFloat(0.0);
        break;
    case JsonType::String:
        m_data = JsonString();
        break;
    case JsonType::Array:
        m_data = JsonArray();
        break;
    case JsonType::Object:
        m_data = JsonObject();
        break;
    }
}


bool Json::Contains(const String& key) const
{
    if (!IsObject())
    {
        return false;
    }
    auto& obj = _GetValue<JsonObject>();
    return obj.Contains(key);
}

Array<String> Json::AllKeys() const
{
    ::Array<String> result;
    if (!IsObject())
    {
        return result;
    }
    auto& obj = _GetValue<JsonObject>();
    for (const auto& [key, value] : obj)
    {
        result.Add(key);
    }

    return result;
}

int64 Json::Size() const
{
    if (!IsArray())
    {
        return -1;
    }
    auto& arr = _GetValue<JsonArray>();
    return arr.Size();
}

String Json::Dump() const
{
    std::ostringstream oss;
    _Serialize(oss, 0, -1);
    return oss.str().c_str();
}

JsonType Json::Type() const
{
    return m_type;
}

JsonBool Json::AsBool() const
{
    return _GetValue<JsonBool>();
}

JsonInt Json::AsInt() const
{
    return _GetValue<JsonInt>();
}

JsonFloat Json::AsFloat() const
{
    return _GetValue<JsonFloat>();
}

JsonString Json::AsString() const
{
    return _GetValue<JsonString>();
}

JsonArray Json::AsArray() const
{
    return _GetValue<JsonArray>();
}

JsonObject Json::AsObject() const
{
    return _GetValue<JsonObject>();
}

bool Json::IsNull() const noexcept
{
    return m_type == JsonType::Null;
}

bool Json::IsBool() const noexcept
{
    return m_type == JsonType::Bool;
}

bool Json::IsInt() const noexcept
{
    return m_type == JsonType::Int;
}

bool Json::IsFloat() const noexcept
{
    return m_type == JsonType::Float;
}

bool Json::IsString() const noexcept
{
    return m_type == JsonType::String;
}

bool Json::IsArray() const noexcept
{
    return m_type == JsonType::Array;
}

bool Json::IsObject() const noexcept
{
    return m_type == JsonType::Object;
}

void Json::Swap(Json& other) noexcept
{
    std::swap(m_type, other.m_type);
    std::swap(m_data, other.m_data);
}

Json::operator bool() const
{
    return !IsNull();
}

Json& Json::operator[](int32 index)
{
    if (!IsArray())
    {
        throw std::runtime_error("JSON type mismatch: expected array");
    }
    auto& arr = _GetValue<JsonArray>();
    if (index < 0 || index >= static_cast<int32>(arr.Size()))
    {
        throw std::out_of_range("Array index out of bounds");
    }
    return arr[index];
}

const Json& Json::operator[](int32 index) const
{
    if (!IsArray())
    {
        throw std::runtime_error("JSON type mismatch: expected array");
    }
    auto& arr = _GetValue<JsonArray>();
    if (index < 0 || index >= static_cast<int32>(arr.Size()))
    {
        throw std::out_of_range("Array index out of bounds");
    }
    return arr[index];
}

Json& Json::operator[](const String& key)
{
    if (!IsObject())
    {
        throw std::runtime_error("JSON type mismatch: expected object");
    }
    auto& obj = _GetValue<JsonObject>();
    return obj[key];
}

const Json& Json::operator[](const String& key) const
{
    if (!IsObject())
    {
        throw std::runtime_error("JSON type mismatch: expected object");
    }
    auto& obj = _GetValue<JsonObject>();
    return obj.At(key);
}

Json& Json::operator[](const char* key)
{
    if (!IsObject())
    {
        throw std::runtime_error("JSON type mismatch: expected object");
    }
    auto& obj = _GetValue<JsonObject>();
    return obj[key];
}

const Json& Json::operator[](const char* key) const
{
    if (!IsObject())
    {
        throw std::runtime_error("JSON type mismatch: expected object");
    }
    auto& obj = _GetValue<JsonObject>();
    if(obj.Contains(key))
    {
        return obj.At(key);
    }
    else
    {
        return Json();
    }
}

std::ostream& operator<<(std::ostream& os, const Json& json)
{
    os << json.Dump();
    return os;
}

bool operator==(const Json& left, const Json& right)
{
    if (left.m_type != right.m_type) {
        return false;
    }

    return std::visit([](auto&& leftValue, auto&& rightValue) -> bool 
        {
            using LeftType = std::decay_t<decltype(leftValue)>;
            using RightType = std::decay_t<decltype(rightValue)>;

            if constexpr (std::is_same_v<LeftType, RightType>)
            {
                if constexpr (std::is_same_v<LeftType, std::monostate>)
                {
                    return true; // 两个都是null
                }
                else
                {
                    return leftValue == rightValue;
                }
            }
            else
            {
                return false;
            }
        }
    , left.m_data, right.m_data);
}

bool operator!=(const Json& left, const Json& right)
{
    return !(left == right);
}

bool operator==(const Json& left, std::nullptr_t)
{
    if (left.IsNull())
    {
        return true;
    }
    return false;
}

bool operator!=(const Json& left, std::nullptr_t)
{
    if (left.IsNull())
    {
        return false;
    }
    return true;
}

bool operator==(std::nullptr_t, const Json& right)
{
    if (right.IsNull())
    {
        return true;
    }
    return false;
}

bool operator!=(std::nullptr_t, const Json& right)
{
    if (right.IsNull())
    {
        return false;
    }
    return true;
}

String Json::_EscapeChar(const String& str) const
{
    String result;
    result.Reserve(str.Size() + 2);
    StringView strView(str.ToCString());
    for (char ch : strView)
    {
        switch (ch)
        {
        case '"': result += "\\\""; break;
        case '\\': result += "\\\\"; break;
        case '\b': result += "\\b"; break;
        case '\f': result += "\\f"; break;
        case '\n': result += "\\n"; break;
        case '\r': result += "\\r"; break;
        case '\t': result += "\\t"; break;
        default:
            if (static_cast<unsigned char>(ch) < 0x20)
            {
                char buf[7];
                snprintf(buf, sizeof(buf), "\\u%04x", ch);
                result += buf;
            }
            else
            {
                result += ch;
            }
        }
    }
    return result;
}

void Json::_Serialize(std::ostream& os, int indentLevel, int indentSize) const
{
    auto printIndent = [&]()
        {
            if (indentSize > 0 && indentLevel > 0)
            {
                os << std::string(indentLevel * indentSize, ' ');
            }
        };

    switch (m_type)
    {
    case JsonType::Null:
    {
        os << "null";
        break;
    }
    case JsonType::Bool:
    {
        os << (std::get<JsonBool>(m_data) ? "true" : "false");
        break;
    }
    case JsonType::Int:
    {
        os << std::get<JsonInt>(m_data);
        break;
    }
    case JsonType::Float:
    {
        os << std::setprecision(std::numeric_limits<double>::max_digits10)
            << std::get<JsonFloat>(m_data);
        break;
    }
    case JsonType::String:
    {
        os << '"' << _EscapeChar(std::get<JsonString>(m_data)) << '"';
        break;
    }
    case JsonType::Array:
    {
        const auto& arr = std::get<JsonArray>(m_data);
        if (arr.IsEmpty())
        {
            os << "[]";
            break;
        }

        os << "[";
        if (indentSize > 0)
        {
            os << "\n";
        }

        for (int32 i = 0; i < arr.Size(); ++i)
        {
            if (i > 0)
            {
                os << ",";
                if (indentSize > 0)
                {
                    os << "\n";
                }
                else
                {
                    os << " ";
                }
            }

            if (indentSize > 0)
            {
                printIndent();
            }
            arr[i]._Serialize(os, indentLevel + 1, indentSize);
        }

        if (indentSize > 0)
        {
            os << "\n";
            if (indentLevel > 0)
            {
                os << std::string((indentLevel - 1) * indentSize, ' ');
            }
        }
        os << "]";
        break;
    }
    case JsonType::Object:
    {
        const auto& obj = std::get<JsonObject>(m_data);
        if (obj.IsEmpty())
        {
            os << "{}";
            break;
        }

        os << "{";
        if (indentSize > 0)
        {
            os << "\n";
        }

        bool first = true;
        for (const auto& pair : obj)
        {
            if (!first)
            {
                os << ",";
                if (indentSize > 0)
                {
                    os << "\n";
                }
                else
                {
                    os << " ";
                }
            }
            first = false;

            if (indentSize > 0)
            {
                printIndent();
            }
            os << '"' << _EscapeChar(pair.first) << '"' << ":";
            if (indentSize > 0)
            {
                os << " ";
            }
            pair.second._Serialize(os, indentLevel + 1, indentSize);
        }

        if (indentSize > 0)
        {
            os << "\n";
            if (indentLevel > 0)
            {
                os << std::string((indentLevel - 1) * indentSize, ' ');
            }
        }
        os << "}";
        break;
    }
    }
}

