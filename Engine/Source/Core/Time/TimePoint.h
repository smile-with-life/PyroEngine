#pragma once
#include "Core.h"
#include "Duration.h"

template<class Clock>
class TimePoint
{
public:
    friend class Clock;
public:
    using clock = Clock::clock;
public:
    TimePoint() = default;

    ~TimePoint() = default;

    TimePoint(const TimePoint& other) = default;

    TimePoint& operator=(const TimePoint& other) = default;

    TimePoint(TimePoint&& other) = default;

    TimePoint& operator=(TimePoint&& other) = default;
public:
    template<class Clock, class Period>
    friend TimePoint<Clock> operator+(const TimePoint<Clock>& left, const Duration<Period>& right);

    template<class Clock>
    friend Nanoseconds operator-(const TimePoint<Clock>& left, const TimePoint<Clock>& right);

    template<class Clock, class Period>
    friend TimePoint operator-(const TimePoint& left, const Duration<Period>& right);

    friend bool operator==(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time == right.m_time;
    }

    friend bool operator!=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time != right.m_time;
    }

    friend bool operator>(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time > right.m_time;
    }

    friend bool operator>=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time >= right.m_time;
    }

    friend bool operator<(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time < right.m_time;
    }

    friend bool operator<=(const TimePoint& left, const TimePoint& right)
    {
        return left.m_time <= right.m_time;
    }
private:
    explicit TimePoint(std::chrono::time_point<clock> time)
        : m_time(time)
    {

    }
private:
    std::chrono::time_point<clock> m_time;
};

template<class Clock, class Period>
inline TimePoint<Clock> operator+(const TimePoint<Clock>& left, const Duration<Period>& right)
{
    return TimePoint<Clock>(left.m_time + right.m_duration);
}

template<class Clock>
inline Nanoseconds operator-(const TimePoint<Clock>& left, const TimePoint<Clock>& right)
{
    using std::chrono::duration_cast;
    auto count = duration_cast<std::chrono::nanoseconds>(left.m_time - right.m_time).count();
    return Nanoseconds(count);
}

template<class Clock, class Period>
inline TimePoint<Clock> operator-(const TimePoint<Clock>& left, const Duration<Period>& right)
{
    return TimePoint(left.m_time - right.m_duration);
}