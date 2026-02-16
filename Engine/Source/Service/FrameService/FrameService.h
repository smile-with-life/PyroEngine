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
class FrameService : public IService
{
public:
    /// <summary>
    /// 初始化
    /// </summary>
    virtual void Init() override;
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick() override;
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit() override;
    /// <summary>
    /// 是否支持动态加载
    /// </summary>
    /// <returns></returns>
    virtual bool IsSupportDynamicReload();
public:
    /// <summary>
    /// 获取当前帧率控制模式
    /// </summary>
    /// <returns>帧率控制模式</returns>
    FramePacingMode GetMode() const;
    /// <summary>
    /// 设置帧率控制模式
    /// </summary>
    /// <param name="mode">要设置的帧率控制模式</param>
    void SetMode(FramePacingMode mode);
    /// <summary>
    /// 获取固定的目标帧率
    /// </summary>
    /// <returns></returns>
    int32 GetFixedFPS() const;
    /// <summary>
    /// 设置固定的目标帧率
    /// <para>仅在 FramePacingMode::Fixed 模式下生效。</para>
    /// </summary>
    /// <param name="fps">要设置的固定帧率值（每秒帧数）</param>
    void SetFixedFPS(int32 fps);
    /// <summary>
    /// 获取当前实际帧率
    /// <para>基于过去一秒内的帧数计算得到的实时帧率。</para>
    /// </summary>
    /// <returns>当前实际运行的帧率（FPS）</returns>
    int32 GetCurrentFPS() const;
    /// <summary>
    /// 执行等待
    /// </summary>
    void Wait(Microseconds time) const;
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