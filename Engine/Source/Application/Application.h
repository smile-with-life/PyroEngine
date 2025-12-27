#pragma once

#include "Core.h"
#include "Service.h"
#include "String/String.h"

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
    Application();

    ~Application();
public:
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
private:
    // 退出标志
    bool m_isQuit = false;
};