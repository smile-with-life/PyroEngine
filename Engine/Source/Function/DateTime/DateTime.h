#pragma once
#include "Core.h"
#include "String/String.h"
#include "Time/TimePoint.h"

// 时区
enum class TimeZone
{
    None
};

enum class DateTimeKind
{
    UTC,    // UTC 时间
    Local,  // 当地时间
};

// 时间和日期
class DateTime
{
public:
    // 默认构造函数
    DateTime() = default;

    // 析构函数
    ~DateTime() = default;

    // 拷贝构造函数
    DateTime(const DateTime& other) = default;

    // 拷贝赋值函数
    DateTime& operator=(const DateTime& other) = default;

    // 移动构造函数
    DateTime(DateTime&& other) = default;

    // 移动赋值函数
    DateTime& operator=(DateTime&& other) = default;

    DateTime(const TimePoint<SystemClock> time, DateTimeKind kind = DateTimeKind::Local);

    DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 milliseconds);
public:
    // 返回一个新的 DateTime，它将指定的年份数加到此实例的值上
    DateTime AddYears(int32 years);

    // 返回一个新的 DateTime，它将指定的月数加到此实例的值上
    DateTime AddMonths(int32 months);

    // 返回一个新的 DateTime，它将指定的天数加到此实例的值上
    DateTime AddDays(int32 days);

    // 返回一个新的 DateTime，它将指定的小时数加到此实例的值上
    DateTime AddHours(int32 hours);

    // 返回一个新的 DateTime，它将指定的分钟数加到此实例的值上
    DateTime AddMinutes(int32 minutes);

    // 返回一个新的 DateTime，它将指定的秒数加到此实例的值上
    DateTime AddSeconds(int32 seconds);

    // 返回一个新的 DateTime，它将指定的秒数加到此实例的值上
    DateTime AddMilliSeconds(int32 milliseconds);

    // 返回一个新的 DateTime，它在此实例上减去指定的年份
    DateTime ReduceYears(int32 years);

    // 返回一个新的 DateTime，它在此实例上减去指定的月数
    DateTime ReduceMonths(int32 months);

    // 返回一个新的 DateTime，它在此实例上减去指定的天数
    DateTime ReduceDays(int32 days);

    // 返回一个新的 DateTime，它在此实例上减去指定的小时
    DateTime ReduceHours(int32 hours);

    // 返回一个新的 DateTime，它在此实例上减去指定的分钟
    DateTime ReduceMinutes(int32 minutes);

    // 返回一个新的 DateTime，它在此实例上减去指定的秒
    DateTime ReduceSeconds(int32 seconds);

    // 返回一个新的 DateTime，它在此实例上减去指定的毫秒
    DateTime ReduceMilliSeconds(int32 milliseconds);

    // 年份
    int32 Year() const;

    // 月份
    int32 Month() const;

    // 天
    int32 Day() const;

    // 小时
    int32 Hour() const;

    // 分钟
    int32 Minute() const;

    // 秒
    int32 Second() const;

    // 毫秒
    int32 MilliSecond() const;

    // 转为字符串
    String ToString();

    // 交换
    void Swap(DateTime& other) noexcept;
public:
    friend bool operator==(const DateTime& left, const DateTime& right);

    friend bool operator!=(const DateTime& left, const DateTime& right);

    friend bool operator>(const DateTime& left, const DateTime& right);

    friend bool operator>=(const DateTime& left, const DateTime& right);

    friend bool operator<(const DateTime& left, const DateTime& right);

    friend bool operator<=(const DateTime& left, const DateTime& right);
public:
    static DateTime Current();

    static DateTime CurrentUTC();

    static int32 Compare(const DateTime& left, const DateTime& right);

    // 返回指定的年份是否为闰年的指示
    static bool IsLeapYear(int32 year);

    // 返回指定年和月中的天数
    static int32 DaysInMonth(int32 year, int32 month);
private:
    void _Normalize(DateTime& time);
private:
    int32 m_year = -1;
    int32 m_month = -1;
    int32 m_day = -1;
    int32 m_hour = -1;
    int32 m_minute = -1;
    int32 m_second = -1;
    int32 m_millisecond = -1;
};