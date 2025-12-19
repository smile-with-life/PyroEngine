#pragma once

#include "Core.h"
#include "Server.h"
#include "String/String.h"
#include "Memory/RAII.h"

struct ApplicationInfo
{
    // 应用名称
    String name;
    // 版本号
    String version;
    // 构建号
    String buildNumber;
    // 开发者
    String developer;
    // 版权信息 
    String copyright;
};

class Application
{
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    int32 Init();

    /// <summary>
    /// 帧更新
    /// </summary>
    void Tick();

    /// <summary>
    /// 退出清理
    /// </summary>
    void Exit();

    /// <summary>
    /// 运行
    /// </summary>
    /// <returns></returns>
    int32 Run();

    /// <summary>
    /// 关于
    /// </summary>
    /// <returns></returns>
    ApplicationInfo About();

    /// <summary>
    /// 是否退出
    /// </summary>
    /// <returns></returns>
    bool IsQuit() const;
public:
    static Application& GetInstance();
protected:
    Application() = default;

    ~Application() = default;
private:
    // 退出标志
    bool m_isQuit = false;
};