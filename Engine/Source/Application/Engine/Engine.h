#pragma once
#include "Core.h"

class Engine
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
};