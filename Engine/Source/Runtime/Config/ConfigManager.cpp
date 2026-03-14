#include "pch.h"

#include "ConfigManager.h"

#include "FileSystem/File.h"

/* ==================== static ==================== */
ConfigManager& ConfigManager::GetInstance()
{
    static ConfigManager instance;
    return instance;
}


/* ==================== public ==================== */
Config ConfigManager::GetConfig(const String& key)
{
    if (m_data.Contains(key))
    {
        return m_data[key];
    }
    return Config();
}

void ConfigManager::SetConfig(const String& key, const Config& value)
{
    
}

bool ConfigManager::Contains(const String& key) const
{
    return m_data.Contains(key);
}

/* ==================== protected ==================== */
ConfigManager::ConfigManager()
{
    // 加载静态配置清单
    Array<String> configNames = _LoadConfigManifest();
    for (const auto& configName : configNames)
    {
        // 加载默认配置
        String defaultPath = m_defaultRoot + configName;
        ConfigFile defaultConfig = _LoadConfigFile(defaultPath);
        if (!defaultConfig.IsValid())
        {
            throw std::runtime_error(std::format("Config File Not Found : {}", defaultPath));
        }
        m_configs.Add(defaultConfig);
        for (const auto& [key, value] : defaultConfig)
        {
            m_data[key] = value;
        }

        // 加载用户配置
        String userPath = m_userRoot + configName;
        ConfigFile userConfig = _LoadConfigFile(userPath);
        if (userConfig.IsValid())
        {
            m_configs.Add(userConfig);
            for (const auto& [key, value] : userConfig)
            {
                m_data[key] = value;
            }
        }
    }
}

/* ==================== private ==================== */
Array<String> ConfigManager::_LoadConfigManifest()
{
    File file(m_defaultRoot + "Config.meta");
    if (file.IsExists())
    {
        FileStream stream = file.Open();
        ByteArray buffer;
        String line;
        Array<String> result;
        bool isValid = false;
        // 查找有效标志
        while (!stream.IsEOF())
        {
            stream.ReadLine(buffer, '\n');
            line = buffer;
            line.Trim();

            if (line.IsEmpty() || line[0] == ';' || line[0] == '#')
            {
                continue;
            }

            // 判断是否为节的头           
            if (line == "[Config Manifest]")
            {
                isValid = true;
                break;
            }
        }

        if (!isValid)
        {
            throw std::runtime_error("Config Manifest File Format Error!");
        }
        
        while (!stream.IsEOF())
        {
            stream.ReadLine(buffer, '\n');
            line = buffer;
            line.Trim();

            if (line.IsEmpty() || line[0] == ';' || line[0] == '#')
            {
                continue;
            }

            if (line.StartWith("\"") && line.EndWith("\""))
            {
                result.Add(line.SubStr(1, line.Size() - 2));
            }
        }
        
        return result;
        
    }
    else
    {
        throw std::runtime_error("Config Manifest File Load Failed!");
    }
}

ConfigFile ConfigManager::_LoadConfigFile(const String& path)
{
    File file(path);
    if (file.IsExists())
    {
        FileStream stream = file.Open();
        ByteArray buffer; 
        stream.ReadAll(buffer);
        return ConfigFile(Json::Parse(buffer));
    }
    return ConfigFile();
}
