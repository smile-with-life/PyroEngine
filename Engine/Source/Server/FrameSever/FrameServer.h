#pragma once
#include "Core.h"
#include "Module/Module.h"
#include "Time/Time.h"

class FrameServer : public CoreModule
{
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;
private:
    // 最大帧率限制
    int32 m_maxFPS = 30;

    // 帧率
    int32 m_FPS = 0;

    // 帧间隔
    Microseconds m_frameTime = 33333;

    // 每秒帧数
    int64 m_frameCount = 0;  

    // 上次更新帧率的时间
    TimePoint<SteadyClock> m_lastUpdateTime;
};