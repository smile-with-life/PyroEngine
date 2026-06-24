#include "pch.h"

#include "Config.h"

/* ==================== public ==================== */
Config::Config(const Json& json)
    : m_value(json)
{

}

bool Config::GetValue(const String& expression, int32& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsInt())
        {
            value = target->AsInt();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, int64& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsInt())
        {
            value = target->AsInt();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, float& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsFloat())
        {
            value = target->AsFloat();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, double& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsFloat())
        {
            value = target->AsFloat();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, bool& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsBool())
        {
            value = target->AsBool();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, String& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsString())
        {
            value = target->AsString();
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

bool Config::GetValue(const String& expression, Config& value) const
{
    if (_ParseExpression(expression))
    {
        const Json* target = _Navigate();
        if (target && target->IsObject())
        {
            value = Config(*target);
            return true;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Config Expression Format Error!");
    }
}

void Config::GetValueOrDefault(const String& expression, int32& value, int32 defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, int64& value, int64 defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, float& value, float defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, double& value, double defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, bool& value, bool defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, String& value, String defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetValueOrDefault(const String& expression, Config& value, Config defaultValue) const
{
    if (!GetValue(expression, value))
    {
        value = defaultValue;
    }
}

void Config::GetArray(const String& expression, Array<int32>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (int64 i = 0; i < target->Size(); ++i) 
    {
        const Json& item = (*target)[i];
        if (item.IsInt()) {
            array.Add(item.AsInt());
        }
    }
}

void Config::GetArray(const String& expression, Array<int64>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (int64 i = 0; i < target->Size(); ++i)
    {
        const Json& item = (*target)[i];
        if (item.IsInt()) {
            array.Add(item.AsInt());
        }
    }
}

void Config::GetArray(const String& expression, Array<float>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (int64 i = 0; i < target->Size(); ++i) 
    {
        const Json& item = (*target)[i];
        if (item.IsFloat()) 
        {
            array.Add(item.AsFloat());
        }
    }
}

void Config::GetArray(const String& expression, Array<double>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (int64 i = 0; i < target->Size(); ++i) 
    {
        const Json& item = (*target)[i];
        if (item.IsFloat()) 
        {
            array.Add(item.AsFloat());
        }
    }
}

void Config::GetArray(const String& expression, Array<bool>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (int64 i = 0; i < target->Size(); ++i) 
    {
        const Json& item = (*target)[i];
        if (item.IsBool()) 
        {
            array.Add(item.AsBool());
        }
    }
}

void Config::GetArray(const String& expression, Array<String>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression)) 
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (size_t i = 0; i < target->Size(); ++i) 
    {
        const Json& item = (*target)[i];
        if (item.IsString()) {
            array.Add(item.AsString());
        }
    }
}

void Config::GetArray(const String& expression, Array<Config>& array) const
{
    array.Clear();
    if (!_ParseExpression(expression))
    {
        throw std::runtime_error("Config Expression Format Error!");
    }

    const Json* target = _Navigate();
    if (!target || !target->IsArray()) return;

    for (size_t i = 0; i < target->Size(); ++i)
    {
        const Json& item = (*target)[i];
        if (item.IsObject()) {
            array.Add(Config(item.AsObject()));
        }
    }
}

bool Config::IsValid() const
{
    return !m_value.IsNull();
}

/* ==================== private ==================== */
bool Config::_ParseExpression(const String& expression) const
{
    m_steps.clear();
    if (expression == "this")
        return true; // 自身
    if (!expression.StartWith("this->"))
        return false; // 错误格式
    // 获取语句
    std::string sentence = expression.RightStr(expression.Size() - 6);
    if (sentence.empty())
        return true; // "this->" 后面为空，视为当前值自身（允许）
    // 遍历语句生成语法树
    int64 pos = 0;
    const int64 length = sentence.size();

    while (pos < length)
    {
        char ch = sentence[pos];

        if (ch == '.') // 点号表示成员访问
        {
            pos++; // 跳过点
            if (pos >= length)
                return false; // 点号后无内容 错误格式

            // 读取成员名，直到遇到下一个点号或左括号或结尾
            int64 start = pos;
            while (pos < length && sentence[pos] != '.' && sentence[pos] != '[')
            {
                pos++;
            }

            if (start == pos)
                return false; // 空成员名 错误格式
            // 获取成员名
            std::string member = sentence.substr(start, pos - start);
            m_steps.emplace_back(member);
        }
        else if (ch == '[')
        {
            // 左括号表示数组索引
            pos++; // 跳过 '['
            if (pos >= length)
                return false; // 缺少数字 错误格式

            // 读取连续的数字
            int64 start = pos;
            while (pos < length && isdigit(static_cast<unsigned char>(sentence[pos])))
                pos++;
            if (start == pos)
                return false; // 没有数字

            // 解析索引值
            int64 index = std::stoul(sentence.substr(start, pos - start));
            if (pos >= length || sentence[pos] != ']')
                return false; // 缺少闭合 ']'
            pos++; // 跳过 ']'

            m_steps.emplace_back(index);
        }
        else
        {
            // 路径以成员名开头（没有前导点）
            // 读取成员名直到遇到点或左括号
            int64 start = pos;
            while (pos < length && sentence[pos] != '.' && sentence[pos] != '[')
                pos++;
            if (start == pos)
                return false; // 不应该发生

            String member = sentence.substr(start, pos - start);
            m_steps.emplace_back(member);
        }
    }

    return true;
}

const Json* Config::_Navigate() const
{
    if (m_value.IsNull())
        return nullptr;

    const Json* current = &m_value;
    for (const auto& step : m_steps)
    {
        if (std::holds_alternative<std::string>(step))
        {
            // 成员访问
            const std::string& key = std::get<std::string>(step);
            if (!current->IsObject() || !current->Contains(key))
                return nullptr;
            current = &(*current)[key];
        }
        else
        {
            // 数组索引
            int64 index = std::get<int64>(step);
            if (!current->IsArray() || index >= current->Size())
                return nullptr;
            current = &(*current)[index];
        }
    }
    return current;
}


