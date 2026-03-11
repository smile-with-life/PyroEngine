#include "pch.h"

#include "ConfigManager.h"

#include "FileSystem/File.h"

/* ==================== static ==================== */
ConfigManager& ConfigManager::GetInstance()
{
    static ConfigManager instance;
    return instance;
}

void ConfigManager::Load(const String& name)
{
    File file(m_root + name + ".config");
    if (file.IsExists())
    {
        FileStream stream = file.Open();
        ByteArray buffer;
        stream.ReadAll(buffer);
        stream.Close();
        Json json = Json::Parse(buffer);
        m_data.Add({ name, json });
    }
    else
    {
        throw std::runtime_error("Config file not found: " + (m_root + name + ".config"));
    }
}

void ConfigManager::Save(const String& name)
{
    String path = m_root + name + ".config";
    if (m_data.Contains(name))
    {
        File file(path);
        if (!file.IsExists())
        {
            file.Create();
        }
        FileStream stream = file.Open();
        ByteArray buffer(m_data[name].Dump());
        stream.WriteAll(buffer);
        stream.Close();
    }
}

/* ==================== public ==================== */
Json ConfigManager::GetValue(const String& name, const String& key)
{
    if (!m_data.Contains(name))
    {
        Load(name); 
    }

    const Json& config = m_data[name];
    if (config.IsObject() && config.AsObject().Contains(key))
    {
        return config[key];
    }
    return Json();
}

void ConfigManager::SetValue(const String& name, const String& key, const Json value)
{
    if (!m_data.Contains(name))
    {
        m_data.Add({ name, Json::Object() });
    }
    m_data[name][key] = value;
    Save(name);
}

bool ConfigManager::HasConfig(const String& name) const
{
    if (!m_data.Contains(name))
    {
        if (!FileSystem::IsExists(m_root + name + ".config"))
        {
            return false;
        }
    }
    return true;
}
