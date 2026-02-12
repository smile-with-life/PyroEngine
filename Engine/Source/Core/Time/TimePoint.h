#pragma once

#include "Core.h"
#include "Duration.h"

#include <chrono>

/// <summary>
/// 时间点
/// </summary>
template<class Clock>
class TimePoint
{
public:
    friend class SystemClock;
    friend class SteadyClock;
public:
    using clock = typename Clock::clock;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr TimePoint() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~TimePoint() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    constexpr TimePoint(const TimePoint& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    constexpr TimePoint& operator=(const TimePoint& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    constexpr TimePoint(TimePoint&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    constexpr TimePoint& operator=(TimePoint&& other) = default;
public:
    /// <summary>
    /// 获取从时钟纪元到当前时间点的纳秒计数
    /// </summary>
    /// <returns>纳秒数</returns>
    constexpr Nanoseconds TimeEpoch() const
    {
        using std::chrono::duration_cast;
        auto count = duration_cast<std::chrono::nanoseconds>(m_time.time_since_epoch()).count();
        return Nanoseconds(count);
    }
public:
    /// <summary>
    /// 时间点 + 时长 = 新的时间点
    /// </summary>
    template<class Clock, class Period>
    constexpr friend TimePoint<Clock> operator+(const TimePoint<Clock>& left, const Duration<Period>& right);
    /// <summary>
    /// 时间点 - 时间点 = 时间间隔（纳秒）
    /// </summary>
    template<class Clock>
    constexpr friend Nanoseconds operator-(const TimePoint<Clock>& left, const TimePoint<Clock>& right);
    /// <summary>
    /// 时间点 - 时长 = 新的时间点
    /// </summary>
    template<class Clock, class Period>
    constexpr friend TimePoint operator-(const TimePoint& left, const Duration<Period>& right);
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time == right.m_time;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time != right.m_time;
    }
    /// <summary>
    /// 小于运算符
    /// </summary>
    constexpr friend bool operator<(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time < right.m_time;
    }
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    constexpr friend bool operator<=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time <= right.m_time;
    }
    /// <summary>
    /// 大于运算符
    /// </summary>
    constexpr friend bool operator>(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time > right.m_time;
    }
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    constexpr friend bool operator>=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time >= right.m_time;
    }
private:
    /// <summary>
    /// 构造函数，仅限友元时钟类调用，保证类型安全
    /// </summary>
    constexpr explicit TimePoint(std::chrono::time_point<clock> time)
        : m_time(time)
    {

    }
private:
    std::chrono::time_point<clock> m_time;
};

/* member */

template<class Clock, class Period>
inline constexpr TimePoint<Clock> operator+(const TimePoint<Clock>& left, const Duration<Period>& right)
{
    return TimePoint<Clock>(left.m_time + right.m_duration);
}

template<class Clock>
inline constexpr Nanoseconds operator-(const TimePoint<Clock>& left, const TimePoint<Clock>& right)
{
    using std::chrono::duration_cast;
    auto count = duration_cast<std::chrono::nanoseconds>(left.m_time - right.m_time).count();
    return Nanoseconds(count);
}

template<class Clock, class Period>
inline constexpr TimePoint<Clock> operator-(const TimePoint<Clock>& left, const Duration<Period>& right)
{
    return TimePoint(left.m_time - right.m_duration);
}