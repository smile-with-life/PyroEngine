#include "pch.h"
#include "DateTime.h"

/* static */
DateTime DateTime::Current()
{
    auto now = SystemClock::Now();
    return DateTime(now, DateTimeKind::Local);
}

DateTime DateTime::CurrentUTC()
{
    auto now = SystemClock::Now();
    return DateTime(now, DateTimeKind::UTC);
}

int32 DateTime::Compare(const DateTime& left, const DateTime& right)
{
    if (left.m_year != right.m_year) 
        return left.m_year - right.m_year;
    if (left.m_month != right.m_month) 
        return left.m_month - right.m_month;
    if (left.m_day != right.m_day) 
        return left.m_day - right.m_day;
    if (left.m_hour != right.m_hour) 
        return left.m_hour - right.m_hour;
    if (left.m_minute != right.m_minute) 
        return left.m_minute - right.m_minute;
    if (left.m_second != right.m_second) 
        return left.m_second - right.m_second;
    if (left.m_millisecond != right.m_millisecond) 
        return left.m_millisecond - right.m_millisecond;
    return 0;
}

bool DateTime::IsLeapYear(int32 year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int32 DateTime::DaysInMonth(int32 year, int32 month)
{
    if (month < 1 || month > 12) 
    {
        return -1;
    }

    static const int32_t daysInMonth[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    if (month == 2 && IsLeapYear(year)) 
    {
        return 29;
    }
    return daysInMonth[month - 1];
}

/* public */
DateTime::DateTime(const TimePoint<SystemClock> time, DateTimeKind kind)
{ 
    auto tt = SystemClock::ToTime(time);

    std::tm tm;
    if (kind == DateTimeKind::UTC) 
    {
        // UTC 时间
        tm = *gmtime(&tt);
    }
    else 
    {
        // 本地时间
        tm = *localtime(&tt);
    }

    m_year = tm.tm_year + 1900;
    m_month = tm.tm_mon + 1;
    m_day = tm.tm_mday;
    m_hour = tm.tm_hour;
    m_minute = tm.tm_min;
    m_second = tm.tm_sec;

    // 计算毫秒部分
    auto duration = time.TimeEpoch();
    auto seconds = ConvertType<Seconds>(duration);
    auto milliseconds = ConvertType<Milliseconds>(duration - seconds);
    m_millisecond = milliseconds.Count();
}

DateTime::DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond)
    : m_year(year), m_month(month), m_day(day)
    , m_hour(hour), m_minute(minute) , m_second(second), m_millisecond(millisecond)
{
    _Normalize(*this);
}

DateTime DateTime::AddYears(int32 years)
{
    DateTime result = *this;
    result.m_year += years;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddMonths(int32 months)
{
    DateTime result = *this;
    result.m_month += months;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddDays(int32 days)
{
    DateTime result = *this;
    result.m_day += days;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddHours(int32 hours)
{
    DateTime result = *this;
    result.m_hour += hours;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddMinutes(int32 minutes)
{
    DateTime result = *this;
    result.m_minute += minutes;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddSeconds(int32 seconds)
{
    DateTime result = *this;
    result.m_second += seconds;
    _Normalize(result);
    return result;
}

DateTime DateTime::AddMilliSeconds(int32 milliseconds)
{
    DateTime result = *this;
    result.m_millisecond += milliseconds;
    _Normalize(result);
    return result;
}

DateTime DateTime::ReduceYears(int32 years)
{
    return AddYears(-years);
}

DateTime DateTime::ReduceMonths(int32 months)
{
    return AddMonths(-months);
}

DateTime DateTime::ReduceDays(int32 days)
{
    return AddDays(-days);
}

DateTime DateTime::ReduceHours(int32 hours)
{
    return AddHours(-hours);
}

DateTime DateTime::ReduceMinutes(int32 minutes)
{
    return AddMinutes(-minutes);
}

DateTime DateTime::ReduceSeconds(int32 seconds)
{
    return AddSeconds(-seconds);
}

DateTime DateTime::ReduceMilliSeconds(int32 milliseconds)
{
    return AddMilliSeconds(-milliseconds);
}

int32 DateTime::Year() const
{
    return m_year;
}

int32 DateTime::Month() const
{
    return m_month;
}

int32 DateTime::Day() const
{
    return m_day;
}

int32 DateTime::Hour() const
{
    return m_hour;
}

int32 DateTime::Minute() const
{
    return m_minute;
}

int32 DateTime::Second() const
{
    return m_second;
}

int32 DateTime::MilliSecond() const
{
    return m_millisecond;
}

String DateTime::ToString()
{
    std::string result = std::format("{0}-{1}-{2} {3}:{4}:{5}", m_year, m_month, m_day, m_hour, m_minute, m_second);
    return String(result);
}

void DateTime::Swap(DateTime& other) noexcept
{
    std::swap(m_year, other.m_year);
    std::swap(m_month, other.m_month);
    std::swap(m_day, other.m_day);
    std::swap(m_hour, other.m_hour);
    std::swap(m_minute, other.m_minute);
    std::swap(m_second, other.m_second);
}

bool operator==(const DateTime& left, const DateTime& right)
{
    return left.m_year == right.m_year &&
           left.m_month == right.m_month &&
           left.m_day == right.m_day &&
           left.m_hour == right.m_hour &&
           left.m_minute == right.m_minute &&
           left.m_second == right.m_second &&
           left.m_millisecond == right.m_millisecond;
}

bool operator!=(const DateTime& left, const DateTime& right)
{
    return !(left == right);
}

bool operator>(const DateTime& left, const DateTime& right)
{
    if (left.m_year != right.m_year) 
        return left.m_year > right.m_year;
    if (left.m_month != right.m_month) 
        return left.m_month > right.m_month;
    if (left.m_day != right.m_day) 
        return left.m_day > right.m_day;
    if (left.m_hour != right.m_hour) 
        return left.m_hour > right.m_hour;
    if (left.m_minute != right.m_minute) 
        return left.m_minute > right.m_minute;
    if (left.m_second != right.m_second) 
        return left.m_second > right.m_second;
    return left.m_millisecond > right.m_millisecond;
}

bool operator>=(const DateTime& left, const DateTime& right)
{
    return left > right || left == right;
}

bool operator<(const DateTime& left, const DateTime& right)
{
    if (left.m_year != right.m_year) 
        return left.m_year < right.m_year;
    if (left.m_month != right.m_month) 
        return left.m_month < right.m_month;
    if (left.m_day != right.m_day) 
        return left.m_day < right.m_day;
    if (left.m_hour != right.m_hour) 
        return left.m_hour < right.m_hour;
    if (left.m_minute != right.m_minute) 
        return left.m_minute < right.m_minute;
    if (left.m_second != right.m_second) 
        return left.m_second < right.m_second;
    return left.m_millisecond < right.m_millisecond;
}

bool operator<=(const DateTime& left, const DateTime& right)
{
    return left < right || left == right;
}

/* private */
void DateTime::_Normalize(DateTime& time)
{
    while (time.m_month > 12) {
        time.m_month -= 12;
        time.m_year++;
    }
    while (time.m_month < 1) {
        time.m_month += 12;
        time.m_year--;
    }

    int32 days = DaysInMonth(time.m_year, time.m_month);
    while (time.m_day > days) 
    {
        time.m_day -= days;
        time.m_month++;
        if (time.m_month > 12) 
        {
            time.m_month = 1;
            time.m_year++;
        }
        days = DaysInMonth(time.m_year, time.m_month);
    }
    while (time.m_day < 1) 
    {
        time.m_month--;
        if (time.m_month < 1)
        {
            time.m_month = 12;
            time.m_year--;
        }
        days = DaysInMonth(time.m_year, time.m_month);
        time.m_day += days;
    }
}