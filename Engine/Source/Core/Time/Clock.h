#pragma once
#include "Core.h"
#include "TimePoint.h"

// 系统时钟
class SystemClock
{
public:
    using clock = std::chrono::system_clock;
public:
    SystemClock() = default;
    ~SystemClock() = default;
public:
    static TimePoint<SystemClock> Now()
    {
        auto time = std::chrono::system_clock::now();
        return TimePoint<SystemClock>(time);
    }
};

// 单调时钟
class SteadyClock
{
public:
    using clock = std::chrono::steady_clock;
public:
    SteadyClock() = default;
    ~SteadyClock() = default;
public:
    static TimePoint<SteadyClock> Now()
    {
        auto time = std::chrono::steady_clock::now();
        return TimePoint<SteadyClock>(time);
    }
};
