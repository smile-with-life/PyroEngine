#pragma once
#include "Core.h"
#include "Service.h"
#include "Time/Time.h"

/// <summary>
/// 帧率控制模式
/// </summary>
enum class FramePacingMode
{
    Unlocked,       // 无限制
    Fixed           // 固定帧率
};

/// <summary>
/// 帧率控制服务
/// <para>在每帧结束时调用</para>
/// </summary>
class FrameService : public CoreService
{
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;

    virtual void LoadSettings() override;

    virtual void SaveSettings() override;

    FramePacingMode GetMode() const;

    int32 GetFixedFPS() const;

    int32 GetCurrentFPS() const;

    void SetMode(FramePacingMode mode);  

    void SetFixedFPS(int32 fps);
private:
    /// <summary>
    /// 帧率限制
    /// </summary>
    int32 m_fixedFPS = 60;

    /// <summary>
    /// 帧率控制模式
    /// </summary>
    FramePacingMode m_mode = FramePacingMode::Unlocked;

    /// <summary>
    /// 帧率
    /// </summary>
    int32 m_FPS = 0;

    /// <summary>
    /// 每秒帧数
    /// </summary>
    int32 m_frameCount = 0;  

    /// <summary>
    /// 每帧间隔时间
    /// </summary>
    Microseconds m_frameTime = 16666; 

    /// <summary>
    /// 上一帧时间
    /// </summary>
    TimePoint<SteadyClock> m_lastFrameTime;

    /// <summary>
    /// 上次帧率更新时间
    /// </summary>
    TimePoint<SteadyClock> m_lastUpdateTime;
};

/// <summary>
/// 最大帧率
/// </summary>
constexpr inline int32 MaxFPS = 240;