#include "pch.h"

#include "FrameService.h"

#include "Runtime.h"
#include "Logger/Logger.h"
#include "Config/ConfigManager.h"

/* ==================== static ==================== */
FrameService& FrameService::GetInstance()
{
    static FrameService instance;
    return instance;
}

/* ==================== public ==================== */
void FrameService::Init()
{
    m_frameTime = Microseconds(1000000) / m_fixedFPS;
    m_lastFrameTime = SteadyClock::Now();
    m_lastUpdateTime = SteadyClock::Now();

    GetConfig();
}

void FrameService::Tick()
{   
    // 获取当前时间，当前帧结束时间
    auto currentTime = SteadyClock::Now();
    // 计算本帧实际耗时（不包含等待时间）
    auto deltaTime = ConvertType<Microseconds>(currentTime - m_lastFrameTime);
    // 判断是否为固定帧率模式
    if (m_mode == FramePacingMode::Fixed)
    {
        // 计算需要等待的时间
        auto waitTime = m_frameTime - deltaTime;
        if (waitTime > Microseconds(0))
        {
            // 等待剩余时间，保证一帧总耗时稳定
            _Wait(waitTime);
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

        GLog->Info("当前帧数:{}", m_FPS);
    }
    // 最后更新上一帧结束时间
    m_lastFrameTime = currentTime;
}

void FrameService::Exit()
{
    
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

void FrameService::GetConfig()
{
    if (GConfigManager->Contains("Frame"))
    {
        auto config = GConfigManager->GetConfig("Frame");

        String mode;
        if (config.GetValue("this->Mode", mode))
        {
            if (mode == "Fixed") SetMode(FramePacingMode::Fixed);
            if (mode == "Unlocked") SetMode(FramePacingMode::Unlocked);
        }
        int32 fps = 0;
        config.GetValueOrDefault("this->FPS", fps, 30);
        SetFixedFPS(fps);
    }
}

/* ==================== private ==================== */
void FrameService::_Wait(Microseconds time) const
{
    
    //std::this_thread::sleep_for(std::chrono::duration<int64, typename Microseconds::period>(time));
    auto start = SteadyClock::Now();

    while (1)
    {
        auto end = SteadyClock::Now();
        auto duration = ConvertType<Microseconds>(end - start);
        if (duration >= time)
        {
            break;
        }
    }
}




