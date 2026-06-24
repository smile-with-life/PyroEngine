#pragma once

#include "Core.h"
#include "Runtime.h"
#include "String/String.h"
#include "FileSystem/File.h"
#include "Logger/Logger.h"
#include "Json/Json.h"
#include "GUID/GUID.h"

#include "Asset/Asset.h"

class AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager() = default;
public:
    template<typename AssetType>
    bool LoadAsset(const String& url, AssetType& asset) const
    {
        File file(url);
        auto stream = file.Open(OpenMode::Default);
        if (!stream.IsValid())
        {
            GLog->Error("打开文件: {} 失败", file.Path());
            return false;
        }

        ByteArray buffer;
        stream.ReadAll(buffer);

        return true;
    }

    template<typename AssetType>
    bool SaveAsset(const AssetType& asset, const std::string& url) const
    {
        

        return true;
    }
public:
    static AssetManager& GetInstance()
    {
        static AssetManager instance;
        return instance;
    }
private:
    HashMap<GUID, RefPtr<Asset>> m_assets;
};