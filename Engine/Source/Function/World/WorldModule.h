#pragma once

#include "Core.h"
#include "Runtime.h"
#include "SubSystem.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Memory/RAII.h"
#include "Config/ConfigManager.h"
#include "Logger/Logger.h"
#include "Module/Module.h"

#include "World/Scene.h"

class WorldModule : public IModule
{
public:
    WorldModule() = default;

    ~WorldModule() = default;
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    virtual void Init() override
    {
        Array<Config> worldConfigs;
        auto config = GConfigManager->GetConfig("Worlds");
        config.GetArray("this", worldConfigs);
        for (const auto& worldConfig : worldConfigs)
        {
            String name;
            String url;
            bool isValid = false;

            worldConfig.GetValue("this->Name", name);
            worldConfig.GetValue("this->Url", url);
            worldConfig.GetValue("this->IsMain", isValid);

            auto world = MakeShared<Scene>(name, url);
            m_scenes.Add(world);

            if(m_activeScene.IsNull())
            {
                m_activeScene = world;
            }
            else
            {
                GLog->Warning("配置有多个世界被标记为主世界，但只会使用第一个被标记的主世界，后续的主世界配置会被忽略");
            }
            
        }
    }
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick() override
    {
        if (!m_activeScene)
        {
            
        }

        m_activeScene->Update();
    }
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit() override
    {

    }
    /// <summary>
    /// 是否支持动态加载
    /// </summary>
    /// <returns></returns>
    virtual bool IsSupportDynamicReload() override
    {
        return false;
    }
public:
    static WorldModule& GetInstance()
    {
        static WorldModule instance;
        return instance;
    }
private:
    /// <summary>
    /// 管理的所有世界
    /// </summary>
    Array<SharedPtr<Scene>> m_scenes;
    /// <summary>
    /// 当前唯一活跃世界
    /// </summary>
    SharedPtr<Scene> m_activeScene;
};