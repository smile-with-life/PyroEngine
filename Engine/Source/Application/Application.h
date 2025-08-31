#pragma once

#include "Core.h"
#include "Console.h"
#include "String.h"

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
    // 初始化
    int32 Init();

    // 帧更新
    void Tick();

    // 退出清理
    void Exit();

    // 是否退出
    bool IsQuit();

    // 运行
    int32 Run();

    // 关于
    ApplicationInfo About();
private:
    // 退出标志
    bool m_isQuit = false;

    // 控制台
    Console* m_console = nullptr;
};