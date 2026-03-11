#pragma once

#include "Core.h"
#include "Container/Map.h"
#include "String/String.h"
#include "Json/Json.h"
#include "FileSystem/FileSystem.h"

/// <summary>
/// 
/// </summary>
class ConfigManager
{
public:
    ~ConfigManager() = default;
public:
    void Load(const String& name); 

    void Save(const String& name);

    Json GetValue(const String& name, const String& key);

    void SetValue(const String& name, const String& key, const Json value);

    bool HasConfig(const String& name) const;
public:
    static ConfigManager& GetInstance();
protected:
    ConfigManager() = default;
private:
    Map<String, Json> m_data;

    String m_root = FileSystem::CurrentPath() + "/Config/";
};