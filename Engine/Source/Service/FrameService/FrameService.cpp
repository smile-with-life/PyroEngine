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
    auto currentFrameTime = SteadyClock::Now();
    // 计算本帧的耗时
    auto deltaTime = ConvertType<Microseconds>(currentFrameTime - m_lastFrameTime);
    // 更新上一帧时间
    m_lastFrameTime = currentFrameTime;
    // 判断是否为固定帧率模式
    if (m_mode == FramePacingMode::Fixed)
    {
        // 计算需要等待的时间
        auto waitTime = m_frameTime - deltaTime;
        if (waitTime > Microseconds(0))
        {
            Thread::Sleep(waitTime);
        }
    }  
    m_frameCount++;
    // 判断是否需要更新帧率
    auto now = SteadyClock::Now();
    auto updateTime = now - m_lastUpdateTime;
    if (updateTime >= Seconds(1))
    {
        m_FPS = m_frameCount;
        m_lastUpdateTime = now;
        m_frameCount = 0;
        GLog->Log(LogLevel::Info, "当前帧数:{}", m_FPS);
    }
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
