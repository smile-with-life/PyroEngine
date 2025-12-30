#include "pch.h"
#include "FrameService.h"
#include "Thread/Thread.h"
#include "GlobalModule.h"

void FrameService::Init()
{
    LoadSettings();
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
        if (waitTime > Microseconds(0))
        {
            // 等待剩余时间，保证一帧总耗时稳定
            Thread::Sleep(waitTime);
        }
    }  
    m_frameCount++;
    // 判断是否需要更新帧率
    auto currentTime = SteadyClock::Now();
    auto updateTime = currentTime - m_lastUpdateTime;
    if (updateTime >= Seconds(1))
    {
        m_FPS = m_frameCount;
        m_lastUpdateTime = currentTime;
        m_frameCount = 0;
        GLog->Log(LogLevel::Info, "当前帧数:{}", m_FPS);
    }
    // 最后更新上一帧结束时间
    m_lastFrameTime = currentTime;
}

void FrameService::Exit()
{
    SaveSettings();
}

void FrameService::LoadSettings()
{

}

void FrameService::SaveSettings()
{

}

FramePacingMode FrameService::GetMode() const
{
    return m_mode;
}

int32 FrameService::GetFixedFPS() const
{
    return m_fixedFPS;
}

int32 FrameService::GetCurrentFPS() const
{
    return m_FPS;
}

void FrameService::SetMode(FramePacingMode mode)
{
    m_mode = mode;
}

void FrameService::SetFixedFPS(int32 fps)
{
    m_fixedFPS = fps > MaxFPS ? MaxFPS : fps;
    m_frameTime = Microseconds(1000000) / m_fixedFPS;
}
