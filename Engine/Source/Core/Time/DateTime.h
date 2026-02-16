#pragma once

#include "Core.h"
#include "String/String.h"
#include "Time/Time.h"

enum class TimeKind
{
    UTC,    // UTC 时间
    Local,  // 当地时间
};

// 时间和日期
class DateTime
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    DateTime() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~DateTime() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    DateTime(const DateTime& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    DateTime& operator=(const DateTime& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    DateTime(DateTime&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    DateTime& operator=(DateTime&& other) = default;
    /// <summary>
    /// 从系统时间点构造 DateTime 对象
    /// </summary>
    /// <param name="time">系统时钟的时间点</param>
    /// <param name="kind">时间类型（UTC 或本地时间）</param>
    DateTime(const TimePoint<SystemClock> time, TimeKind kind = TimeKind::Local);
    /// <summary>
    /// 从具体的日期时间值构造 DateTime 对象
    /// </summary>
    /// <param name="year">年份（1-9999）</param>
    /// <param name="month">月份（1-12）</param>
    /// <param name="day">日期（1-31，根据月份和年份有效）</param>
    /// <param name="hour">小时（0-23）</param>
    /// <param name="minute">分钟（0-59）</param>
    /// <param name="second">秒（0-59）</param>
    /// <param name="milliseconds">毫秒（0-999）</param>
    DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 milliseconds);
public:
    /// <summary>
    /// 获取年份
    /// </summary>
    /// <returns>年份（1-9999），无效对象返回 -1</returns>
    int32 Year() const;
    /// <summary>
    /// 获取月份
    /// </summary>
    /// <returns>月份（1-12），无效对象返回 -1</returns>
    int32 Month() const;
    /// <summary>
    /// 获取日期（当月中的第几天）
    /// </summary>
    /// <returns>日期（1-31），无效对象返回 -1</returns>
    int32 Day() const;
    /// <summary>
    /// 获取小时
    /// </summary>
    /// <returns>小时（0-23），无效对象返回 -1</returns>
    int32 Hour() const;
    /// <summary>
    /// 获取分钟
    /// </summary>
    /// <returns>分钟（0-59），无效对象返回 -1</returns>
    int32 Minute() const;
    /// <summary>
    /// 获取秒
    /// </summary>
    /// <returns>秒（0-59），无效对象返回 -1</returns>
    int32 Second() const;
    /// <summary>
    /// 获取毫秒
    /// </summary>
    /// <returns>毫秒（0-999），无效对象返回 -1</returns>
    int32 MilliSecond() const;
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的年份数加到此实例上
    /// </summary>
    /// <param name="years">要增加的年份数（可以为负数）</param>
    /// <returns>增加年份后的新 DateTime 对象</returns>
    DateTime AddYears(int32 years);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的月数加到此实例上
    /// </summary>
    /// <param name="months">要增加的月数（可以为负数）</param>
    /// <returns>增加月份后的新 DateTime 对象</returns>
    DateTime AddMonths(int32 months);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的天数加到此实例上
    /// </summary>
    /// <param name="days">要增加的天数（可以为负数）</param>
    /// <returns>增加天数后的新 DateTime 对象</returns>
    DateTime AddDays(int32 days);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的小时数加到此实例上
    /// </summary>
    /// <param name="hours">要增加的小时数（可以为负数）</param>
    /// <returns>增加小时后的新 DateTime 对象</returns>
    DateTime AddHours(int32 hours);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的分钟数加到此实例上
    /// </summary>
    /// <param name="minutes">要增加的分钟数（可以为负数）</param>
    /// <returns>增加分钟后的新 DateTime 对象</returns>
    DateTime AddMinutes(int32 minutes);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的秒数加到此实例上
    /// </summary>
    /// <param name="seconds">要增加的秒数（可以为负数）</param>
    /// <returns>增加秒后的新 DateTime 对象</returns>
    DateTime AddSeconds(int32 seconds);
    /// <summary>
    /// 返回一个新的 DateTime 对象，将指定的毫秒数加到此实例上
    /// </summary>
    /// <param name="milliseconds">要增加的毫秒数（可以为负数）</param>
    /// <returns>增加毫秒后的新 DateTime 对象</returns>
    DateTime AddMilliSeconds(int32 milliseconds);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的年份数
    /// </summary>
    /// <param name="years">要减去的年份数</param>
    /// <returns>减少年份后的新 DateTime 对象</returns>
    DateTime ReduceYears(int32 years);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的月数
    /// </summary>
    /// <param name="months">要减去的月数</param>
    /// <returns>减少月份后的新 DateTime 对象</returns>
    DateTime ReduceMonths(int32 months);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的天数
    /// </summary>
    /// <param name="days">要减去的天数</param>
    /// <returns>减少天数后的新 DateTime 对象</returns>
    DateTime ReduceDays(int32 days);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的小时数
    /// </summary>
    /// <param name="hours">要减去的小时数</param>
    /// <returns>减少小时后的新 DateTime 对象</returns>
    DateTime ReduceHours(int32 hours);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的分钟数
    /// </summary>
    /// <param name="minutes">要减去的分钟数</param>
    /// <returns>减少分钟后的新 DateTime 对象</returns>
    DateTime ReduceMinutes(int32 minutes);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的秒数
    /// </summary>
    /// <param name="seconds">要减去的秒数</param>
    /// <returns>减少秒后的新 DateTime 对象</returns>
    DateTime ReduceSeconds(int32 seconds);
    /// <summary>
    /// 返回一个新的 DateTime 对象，从此实例减去指定的毫秒数
    /// </summary>
    /// <param name="milliseconds">要减去的毫秒数</param>
    /// <returns>减少毫秒后的新 DateTime 对象</returns>
    DateTime ReduceMilliSeconds(int32 milliseconds);
    /// <summary>
    /// 将当前 DateTime 对象转换为字符串表示
    /// </summary>
    /// <returns>格式为 "YYYY-MM-DD HH:MM:SS.ms" 的字符串</returns>
    String ToString();
    /// <summary>
    /// 交换两个 DateTime 对象的内容
    /// </summary>
    /// <param name="other">要交换的另一个 DateTime 对象</param>
    void Swap(DateTime& other) noexcept;
public:
    /// <summary>
    /// 相等运算符
    /// </summary>
    friend bool operator==(const DateTime& left, const DateTime& right);
    /// <summary>
    /// 不等运算符
    /// </summary>
    friend bool operator!=(const DateTime& left, const DateTime& right);
    /// <summary>
    /// 小于运算符
    /// </summary>
    friend bool operator<(const DateTime& left, const DateTime& right);
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    friend bool operator<=(const DateTime& left, const DateTime& right);
    /// <summary>
    /// 大于运算符
    /// </summary>
    friend bool operator>(const DateTime& left, const DateTime& right);
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    friend bool operator>=(const DateTime& left, const DateTime& right);
public:
    /// <summary>
    /// 获取当前本地时间
    /// </summary>
    /// <returns>表示当前本地时间的 DateTime 对象</returns>
    static DateTime Current();
    /// <summary>
    /// 获取当前 UTC 时间
    /// </summary>
    /// <returns>表示当前 UTC 时间的 DateTime 对象</returns>
    static DateTime CurrentUTC();
    /// <summary>
    /// 判断指定的年份是否为闰年
    /// </summary>
    /// <param name="year">要检查的年份</param>
    /// <returns>如果是闰年返回 true，否则返回 false</returns>
    static bool IsLeapYear(int32 year);
    /// <summary>
    /// 获取指定年份和月份的天数
    /// </summary>
    /// <param name="year">年份</param>
    /// <param name="month">月份（1-12）</param>
    /// <returns>该月的天数（28-31）</returns>
    static int32 DaysInMonth(int32 year, int32 month);
private:
    /// <summary>
    /// 标准化 DateTime 对象
    /// <para>处理进位/借位，确保各字段在有效范围内</para>
    /// </summary>
    /// <param name="time">要标准化的 DateTime 对象引用</param>
    void _Normalize(DateTime& time);
private:
    /// <summary>
    /// 年份
    /// </summary>
    int32 m_year = -1;
    /// <summary>
    /// 月份（1-12）
    /// </summary>
    int32 m_month = -1;
    /// <summary>
    /// 日期（1-31）
    /// </summary>
    int32 m_day = -1;
    /// <summary>
    /// 小时（0-23）
    /// </summary>
    int32 m_hour = -1;
    /// <summary>
    /// 分钟（0-59）
    /// </summary>
    int32 m_minute = -1;
    /// <summary>
    /// 秒（0-59）
    /// </summary>
    int32 m_second = -1;
    /// <summary>
    /// 毫秒（0-999）
    /// </summary>
    int32 m_millisecond = -1;
};