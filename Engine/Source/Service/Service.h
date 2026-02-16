#pragma once

#include "Core.h"

class IService
{
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    virtual void Init() = 0;
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick() = 0;
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit() = 0;
    /// <summary>
    /// 是否支持动态加载
    /// </summary>
    /// <returns></returns>
    virtual bool IsSupportDynamicReload() = 0;
};