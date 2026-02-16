#include "pch.h"

#include "FrameService.h"

#include "Module.h"

void FrameService::Init()
{
    m_frameTime = Microseconds(1000000) / m_fixedFPS;
    m_lastFrameTime = SteadyClock::Now();
    m_lastUpdateTime = SteadyClock::Now();
}

void FrameService::Tick()
{   
    // 获取当前时间
    auto currentTime = SteadyClock::Now();
    // 计算本帧实际耗时
    auto deltaTime = ConvertType<Microseconds>(currentTime - m_lastFrameTime);
    // 判断是否为固定帧率模式
    if (m_mode == FramePacingMode::Fixed)
    {
        // 计算需要等待的时间
        auto waitTime = m_frameTime - deltaTime;
        if (waitTime > Microseconds(1000))
        {
            // 等待剩余时间，保证一帧总耗时稳定
            Wait(waitTime);
        }
    }  
    m_frameCount++;
    // 判断是否需要更新帧率
    currentTime = SteadyClock::Now();
    auto updateTime = currentTime - m_lastUpdateTime;
    if (updateTime >= Seconds(1))
    {
        m_FPS = m_frameCount;
        m_lastUpdateTime = currentTime;
        m_frameCount = 0;
    }
    // 最后更新上一帧结束时间
    m_lastFrameTime = currentTime;
}

void FrameService::Exit()
{
    
}

bool FrameService::IsSupportDynamicReload()
{
    return false;
}

FramePacingMode FrameService::GetMode() const
{
    return m_mode;
}

void FrameService::SetMode(FramePacingMode mode)
{
    m_mode = mode;
}

int32 FrameService::GetFixedFPS() const
{
    return m_fixedFPS;
}

void FrameService::SetFixedFPS(int32 fps)
{
    if (fps > 0)
    {
        m_fixedFPS = fps;
        m_frameTime = Microseconds(1000000) / m_fixedFPS;
    }
}

int32 FrameService::GetCurrentFPS() const
{
    return m_FPS;
}

void FrameService::Wait(Microseconds time) const
{
    std::this_thread::sleep_for(std::chrono::duration<int64, typename Microseconds::period>(time));
}




