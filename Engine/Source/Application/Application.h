#pragma once

#include "Core/Core.h"

struct ApplicationInfo
{
    
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
};