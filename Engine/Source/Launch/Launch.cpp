#include "pch.h"

#include "Core.h"
#include "Module.h"
#include "Application.h"
#include "CommandLineArgs/CommandLineArgs.h"

/// <summary>
/// 
/// </summary>
/// <param name="cmdArgs"></param>
/// <returns></returns>
int32 EngineMain(const CommandLineArgs& cmdArgs)
{
    // 加载核心模块
    PreInitCoreModule();

    // 获取应用程序单例
    Application& app = Application::Create();

    // 应用程序初始化
    int32 error = app.Init();

    // 应用程序主循环
    while (!app.IsQuit())
    {
        app.Tick();
    }

    // 应用程序退出清理
    app.Exit();

    return error;
}