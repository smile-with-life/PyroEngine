#include "pch.h"

#include "Core.h"
#include "Runtime.h"
#include "Application.h"

/// <summary>
/// 
/// </summary>
/// <param name="cmdArgs"></param>
/// <returns></returns>
int32 EngineMain()
{
    // 解析命令行参数判断要启动的应用程序类型
    Application::Type type = Application::None;
    if (GCommandLineArgs->HasArg("-type"))
    {
        String value = GCommandLineArgs->GetArgValue("-type");
        if (value == "Game")
        {
            type = Application::Game;
        }
        if (value == "Editor")
        {
            type = Application::Editor;
        }
        if (value == "App")
        {
            type = Application::App;
        }
        if (value == "Server")
        {
            type = Application::Server;
        }
    }

    // 获取应用程序单例
    Application& app = Application::Create(type);

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