#include "pch.h"

#include "ConfigFile.h"

/* ==================== public ==================== */
ConfigFile::ConfigFile(const Json& json)
{
    _ParseConfig(json);
}

bool ConfigFile::IsValid() const
{
    return m_data.Size() == 0 ? false : true;
}

Config ConfigFile::operator[](const String& key) const
{
    return m_data.At(key);
}

/* ==================== private ==================== */
void ConfigFile::_ParseConfig(const Json& json)
{
    if (json.Contains("__meta__"))
    {
        m_name = json["__meta__"]["Name"].AsString();
        m_version = json["__meta__"]["Version"].AsString();
        m_generator = json["__meta__"]["Generator"].AsString();
        Array<String> keys = json.AllKeys();
        for (const auto& key : keys)
        {
            if (key != "__meta__")
            {
                Config value(json[key]);
                m_data.Add({ key, value });
            }
        }
    }
    else
    {
        throw std::runtime_error("Config Parse Error!");
    }
}