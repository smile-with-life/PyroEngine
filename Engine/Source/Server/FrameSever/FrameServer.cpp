#include "pch.h"
#include "FrameServer.h"

void FrameServer::Init()
{
    m_lastUpdateTime = SteadyClock::Now();
}

void FrameServer::Tick()
{
    auto currentTime = SteadyClock::Now();

    auto currentFrameTime = ConvertType<Microseconds>(currentTime - m_lastUpdateTime);
    auto waitTime = m_frameTime - currentFrameTime;
    if (waitTime > Microseconds(0))
    {
        auto start = SteadyClock::Now();
        while (1)
        {
            auto end = SteadyClock::Now(); 
            auto duration = ConvertType<Microseconds>(end - start);
            if (duration >= waitTime)
            {
                break;
            }
        }
    }
    m_frameCount++;

    auto now = SteadyClock::Now();
    auto updateTime = now - m_lastUpdateTime;
    if (updateTime >= Seconds(1))
    {
        m_FPS = m_frameCount;
        m_lastUpdateTime = now;
        m_frameCount = 0;
    }
}

void FrameServer::Exit()
{

}