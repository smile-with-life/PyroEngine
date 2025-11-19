#pragma once
#include "Core.h"
#include <chrono>

template<class Clock>
class TimePoint;

template<class Period>
class Duration
{
public:
    template <typename Clock>
    friend class TimePoint;
public:
    using period = Period;
    using type = std::chrono::duration<int64_t, Period>;
public:
    Duration() = default;

    ~Duration() = default;

    Duration(const Duration& other) = default;

    Duration& operator=(const Duration& other) = default;

    Duration(Duration&& other) = default;

    Duration& operator=(Duration&& other) = default;

    Duration(int64 count)
        : m_duration(count)
    {

    }
public:
    int64_t Count() const
    {
        return m_duration.count();
    }

    template<class ToDuration, class Period>
    friend constexpr auto ConvertType(const Duration<Period>& duration);
public:
    template<class Period1, class Period2>
    friend constexpr auto operator+(const Duration<Period1>& left, const Duration<Period2>& right);

    constexpr Duration& operator+=(const Duration& other)
    {
        m_duration += other.m_duration;
        return *this;
    }

    template<class Period1, class Period2>
    friend constexpr auto operator-(const Duration<Period1>& left, const Duration<Period2>& right);

    constexpr Duration& operator-=(const Duration& other)
    {
        m_duration -= other.m_duration;
        return *this;
    }

    template<class Period>
    friend constexpr auto operator*(const Duration<Period>& left, int64 right);

    template<class Period>
    friend constexpr auto operator*(int64 left, const Duration<Period>& right);

    constexpr Duration& operator*=(const Duration& other)
    {
        m_duration *= other.m_duration;
        return *this;
    }

    template<class Period1, class Period2>
    friend constexpr auto operator/(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period>
    friend constexpr auto operator/(const Duration<Period>& left, int64 right);

    constexpr Duration& operator/=(const Duration& other)
    {
        m_duration /= other.m_duration;
        return *this;
    }

    template<class Period1, class Period2>
    friend constexpr auto operator%(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period>
    friend constexpr auto operator%(const Duration<Period>& left, int64 right);

    constexpr Duration& operator%=(const Duration& other)
    {
        m_duration %= other.m_duration;
        return *this;
    }

    template<class Period1, class Period2>
    friend constexpr bool operator==(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period1, class Period2>
    friend constexpr bool operator!=(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period1, class Period2>
    friend constexpr bool operator>(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period1, class Period2>
    friend constexpr bool operator>=(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period1, class Period2>
    friend constexpr bool operator<(const Duration<Period1>& left, const Duration<Period2>& right);

    template<class Period1, class Period2>
    friend constexpr bool operator<=(const Duration<Period1>& left, const Duration<Period2>& right);
private:
    std::chrono::duration<int64_t, Period> m_duration;
};

template<class ToDuration, class Period>
inline constexpr auto ConvertType(const Duration<Period>& duration)
{
    using std::chrono::duration_cast;
    auto count = duration_cast<typename ToDuration::type>(duration.m_duration).count();
    return ToDuration(count);
}

template<class Period1, class Period2>
inline constexpr auto operator+(const Duration<Period1>& left, const Duration<Period2>& right)
{
    using std::chrono::duration_cast;
    using CommonType = typename std::common_type<typename Duration<Period1>::type, typename Duration<Period2>::type>::type;// 获取共同类型
    using Period = typename CommonType::period;

    auto count = duration_cast<CommonType>(left.m_duration).count() + duration_cast<CommonType>(right.m_duration).count();
    return Duration<Period>(count);
}

template<class Period1, class Period2>
inline constexpr auto operator-(const Duration<Period1>& left, const Duration<Period2>& right)
{
    using std::chrono::duration_cast;
    using CommonType = typename std::common_type<typename Duration<Period1>::type, typename Duration<Period2>::type>::type;// 获取共同类型
    using Period = CommonType::period;

    auto count = duration_cast<CommonType>(left.m_duration).count() - duration_cast<CommonType>(right.m_duration).count();
    return Duration<Period>(count);
}

template<class Period>
inline constexpr auto operator*(const Duration<Period>& left, int64 right)
{
    auto count = left.Count() * right;
    return Duration<Period>(count);
}

template<class Period>
inline constexpr auto operator*(int64 left, const Duration<Period>& right)
{
    auto count = left * right.Count();
    return Duration<Period>(count);
}

template<class Period1, class Period2>
inline constexpr auto operator/(const Duration<Period1>& left, const Duration<Period2>& right)
{
    using std::chrono::duration_cast;
    using CommonType = typename std::common_type<typename Duration<Period1>::type, typename Duration<Period2>::type>::type;// 获取共同类型
    using Period = typename CommonType::period;

    auto count = duration_cast<CommonType>(left.m_duration).count() / duration_cast<CommonType>(right.m_duration).count();
    return Duration<Period>(count);
}

template<class Period>
inline constexpr auto operator/(const Duration<Period>& left, int64 right)
{
    auto count = left.Count() / right;
    return Duration<Period>(count);
}

template<class Period1, class Period2>
inline constexpr auto operator%(const Duration<Period1>& left, const Duration<Period2>& right)
{
    using std::chrono::duration_cast;
    using CommonType = typename std::common_type<typename Duration<Period1>::type, typename Duration<Period2>::type>::type;// 获取共同类型
    using Period = typename CommonType::period;

    auto count = duration_cast<CommonType>(left.m_duration).count() % duration_cast<CommonType>(right.m_duration).count();
    return Duration<Period>(count);
}

template<class Period>
inline constexpr auto operator%(const Duration<Period>& left, int64 right)
{
    auto count = left.Count() % right;
    return Duration<Period>(count);
}

template<class Period1, class Period2>
inline constexpr bool operator==(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration == right.m_duration;
}

template<class Period1, class Period2>
inline constexpr bool operator!=(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration != right.m_duration;
}

template<class Period1, class Period2>
inline constexpr bool operator>(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration > right.m_duration;
}

template<class Period1, class Period2>
inline constexpr bool operator>=(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration >= right.m_duration;
}

template<class Period1, class Period2>
inline constexpr bool operator<(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration < right.m_duration;
}

template<class Period1, class Period2>
inline constexpr bool operator<=(const Duration<Period1>& left, const Duration<Period2>& right)
{
    return left.m_duration <= right.m_duration;
}

using Years = Duration<std::ratio<31556952>>;
using Months = Duration<std::ratio<2629746>>;
using Weeks = Duration<std::ratio<604800>>;
using Days = Duration<std::ratio<86400>>;
using Hours = Duration<std::ratio<86400>>;
using Minutes = Duration<std::ratio<86400>>;
using Seconds = Duration<std::ratio<1>>;
using Milliseconds = Duration<std::ratio<1, 1000>>;
using Microseconds = Duration<std::ratio<1, 1000000>>;
using Nanoseconds = Duration<std::ratio<1, 1000000000>>;