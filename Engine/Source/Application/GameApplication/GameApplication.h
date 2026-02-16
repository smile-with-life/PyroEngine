#pragma once

#include "Core.h"
#include "Application.h"

class GameApplication : public Application
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    GameApplication();
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~GameApplication();
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    virtual int32 Init();
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick();
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit();
    /// <summary>
    /// 关于应用程序
    /// </summary>
    /// <returns></returns>
    virtual ApplicationInfo About();
private:

};