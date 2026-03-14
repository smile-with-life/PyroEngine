#pragma once

#include "Core.h"
#include "Container/Map.h"
#include "String/String.h"
#include "Json/Json.h"
#include "FileSystem/FileSystem.h"
#include "Config/Config.h"
#include "Config/ConfigFile.h"

/// <summary>
/// 
/// </summary>
class ConfigManager
{
public:
    ~ConfigManager() = default;
public:
    Config GetConfig(const String& key);

    void SetConfig(const String& key, const Config& value);

    bool Contains(const String& key) const;
public:
    static ConfigManager& GetInstance();
protected:
    ConfigManager();
private:
    Array<String> _LoadConfigManifest();

    ConfigFile _LoadConfigFile(const String& path);
private:
    /// <summary>
    /// 默认的配置文件根路径
    /// </summary>
    String m_defaultRoot = FileSystem::CurrentPath() + "/Config/";
    /// <summary>
    /// 用户修改的配置文件根路径
    /// </summary>
    String m_userRoot = FileSystem::CurrentPath() + "Saved/Config/";
    /// <summary>
    /// 已加载的配置文件
    /// </summary>
    Array<ConfigFile> m_configs;
    /// <summary>
    /// 配置缓存
    /// </summary>
    Map<ConfigKey, Config> m_data;
};