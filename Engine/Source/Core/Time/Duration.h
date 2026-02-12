#pragma once

#include "Core.h"

#include <chrono>

template<class Clock>
class TimePoint;

/// <summary>
/// 时间段
/// </summary>
template<class Period>
class Duration
{
public:
    template <typename Clock>
    friend class TimePoint;
public:
    using period = Period;
    using type = std::chrono::duration<int64, Period>;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Duration() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Duration() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    Duration(const Duration& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    Duration& operator=(const Duration& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    Duration(Duration&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    Duration& operator=(Duration&& other) = default;
    /// <summary>
    /// 构造函数，从整数计数值构造时长
    /// </summary>
    /// <param name="count">以 Period 为单位的计数值</param>
    Duration(int64 count)
        : m_duration(count)
    {

    }
public:
    /// <summary>
    /// 返回刻度计数
    /// </summary>
    /// <returns>以 Period 为单位的计数值</returns>
    int64 Count() const
    {
        return m_duration.count();
    }
    /// <summary>
    /// 单位转换函数
    /// </summary>
    /// <typeparam name="ToDuration"></typeparam>
    /// <typeparam name="Period"></typeparam>
    /// <param name="duration"></param>
    /// <returns></returns>
    template<class ToDuration, class Period>
    constexpr friend auto ConvertType(const Duration<Period>& duration);
public:
    /// <summary>
    /// 隐式转换为 std::chrono::duration
    /// 允许 Duration 无缝用于接收 std::chrono 参数的接口
    /// </summary>
    operator std::chrono::duration<int64, Period>() const
    {
        return m_duration;
    }
    /// <summary>
    /// 前置自增运算符
    /// </summary>
    constexpr Duration& operator++()
    {
        ++m_duration;  
        return *this;
    }
    /// <summary>
    /// 后置自增运算符
    /// </summary>
    constexpr Duration operator++(int)
    {
        return m_duration++;
    }
    /// <summary>
    /// 前置自减运算符
    /// </summary>
    constexpr Duration& operator--()
    {
        --m_duration;
        return *this;
    }
    /// <summary>
    /// 后置自减运算符
    /// </summary>
    constexpr Duration operator--(int)
    {
        return m_duration--;
    }
    /// <summary>
    /// 加法运算符
    /// </summary>
    template<class Period1, class Period2>
    friend constexpr auto operator+(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    constexpr Duration& operator+=(const Duration& other)
    {
        m_duration += other.m_duration;
        return *this;
    }
    /// <summary>
    /// 减法运算符
    /// </summary>
    template<class Period1, class Period2>
    friend constexpr auto operator-(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 减法赋值运算符
    /// </summary>
    constexpr Duration& operator-=(const Duration& other)
    {
        m_duration -= other.m_duration;
        return *this;
    }
    /// <summary>
    /// 乘法运算符（左乘）
    /// </summary>
    template<class Period>
    constexpr friend auto operator*(const Duration<Period>& left, int64 right);
    /// <summary>
    /// 乘法运算符（右乘）
    /// </summary>
    template<class Period>
    constexpr friend auto operator*(int64 left, const Duration<Period>& right);
    /// <summary>
    /// 乘法赋值运算符
    /// </summary>
    constexpr Duration& operator*=(int64 scalar)
    {
        m_duration *= scalar;
        return *this;
    }
    /// <summary>
    /// 除法运算符（左除）
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend auto operator/(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 除法运算符（右除）
    /// </summary>
    template<class Period>
    constexpr friend auto operator/(const Duration<Period>& left, int64 right);
    /// <summary>
    /// 除法赋值运算符
    /// </summary>
    constexpr Duration& operator/=(int64 scalar)
    {
        m_duration /= scalar;
        return *this;
    }
    /// <summary>
    /// 取模运算符（时长%时长）
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend auto operator%(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 取模运算符（时长%标量）
    /// </summary>
    template<class Period>
    constexpr friend auto operator%(const Duration<Period>& left, int64 right);
    /// <summary>
    /// 取模赋值运算符（时长对时长取模）
    /// </summary>
    constexpr Duration& operator%=(const Duration& other)
    {
        m_duration %= other.m_duration;
        return *this;
    }
    /// <summary>
    /// 取模赋值运算符（时长对标量取模）
    /// </summary>
    constexpr Duration& operator%=(int64 scalar)
    {
        m_duration %= scalar;
        return *this;
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator==(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 不等运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator!=(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 小于运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator<(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator<=(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 大于运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator>(const Duration<Period1>& left, const Duration<Period2>& right);
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    template<class Period1, class Period2>
    constexpr friend bool operator>=(const Duration<Period1>& left, const Duration<Period2>& right);
private:
    std::chrono::duration<int64, Period> m_duration;
};

/* member */

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
using Hours = Duration<std::ratio<3600>>;
using Minutes = Duration<std::ratio<60>>;
using Seconds = Duration<std::ratio<1>>;
using Milliseconds = Duration<std::ratio<1, 1000>>;
using Microseconds = Duration<std::ratio<1, 1000000>>;
using Nanoseconds = Duration<std::ratio<1, 1000000000>>;