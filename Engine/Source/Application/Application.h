#pragma once
#include "Core.h"
#include "String/String.h"

enum class ApplicationType
{
    None,
    Game,
    Editor,
    App,
    Server
};

/// <summary>
/// 应用程序信息
/// </summary>
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

/// <summary>
/// 应用程序
/// </summary>
class Application
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Application();
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Application();
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    int32 Init();
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    void Tick();
    /// <summary>
    /// 退出清理
    /// </summary>
    void Exit();
    /// <summary>
    /// 关于应用程序
    /// </summary>
    /// <returns></returns>
    ApplicationInfo About();
    /// <summary>
    /// 是否退出
    /// </summary>
    /// <returns></returns>
    bool IsQuit() const;
public:
    /// <summary>
    /// 创建指定类型的应用程序实例
    /// </summary>
    /// <param name="type">应用程序类型</param>
    /// <returns>创建的应用程序实例引用</returns>
    static Application& Create(ApplicationType type = ApplicationType::None);
private:
    // 退出标志
    bool m_isQuit = false;
};