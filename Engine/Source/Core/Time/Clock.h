#pragma once

#include "Core.h"
#include "TimePoint.h"

#include <chrono>

/// <summary>
/// 系统时钟
/// </summary>
class SystemClock
{
public:
    using clock = std::chrono::system_clock;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr SystemClock() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~SystemClock() = default;
public:
    /// <summary>
    /// 将系统时钟时间点转换为 Unix 时间戳
    /// </summary>
    /// <param name="time">系统时钟的时间点</param>
    /// <returns>Unix 时间戳</returns>
    constexpr static std::time_t ToTime(TimePoint<SystemClock> time)
    {
        return std::chrono::system_clock::to_time_t(time.m_time);
    }
    /// <summary>
    /// 获取当前系统时间
    /// </summary>
    /// <returns>时间点</returns>
    constexpr static TimePoint<SystemClock> Now()
    {
        auto time = std::chrono::system_clock::now();
        return TimePoint<SystemClock>(time);
    }
};

/// <summary>
/// 单调时钟
/// </summary>
class SteadyClock
{
public:
    using clock = std::chrono::steady_clock;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr SteadyClock() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~SteadyClock() = default;
public:
    /// <summary>
    /// 获取当前单调时间
    /// </summary>
    /// <returns>时间点</returns>
    constexpr static TimePoint<SteadyClock> Now()
    {
        auto time = std::chrono::steady_clock::now();
        return TimePoint<SteadyClock>(time);
    }
};