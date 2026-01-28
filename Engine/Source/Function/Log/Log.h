#pragma once
#include "Core.h"
#include "String/String.h"
#include "Time/Time.h"
#include "Thread/Thread.h"
#include "Diagnosis/SourceInfo.h"
#include "Console/Console.h"
#include "DateTime/DateTime.h"

enum class LogLevel : uint16
{
    Off,        // 全部关闭
    Verbose,    // 详情：指明程序运行细节
    Debug,      // 调试：指明调试过程中的事件信息
    Info,       // 信息：指明运行过程中的重要信息
    Warning,    // 警告：指明可能潜在的危险状况
    Error,      // 错误：指明错误事件
    Fatal,      // 致命：指明非常严重的可能会导致应用终止执行错误事件
    All         // 全部开启
};

enum class LogLight : uint16
{
    None,
    Red,
    Orange,
    Yellow,
    Green,
    Cyan,
    Blue,
    Purple
};

struct LogEntry
{
    TimePoint<SystemClock> Time;        // 产生日志消息的时间
    SourceInfo Source;                  // 源代码信息
    ThreadId Id;                        // 产生日志消息的线程ID
    LogLevel Level;                     // 日志消息的级别
    LogLight Light;                     // 日志颜色
    String Message;                     // 日志消息的内容
};

/* 包装代码位置信息 */
template<class Type>
class FormatWithSource
{
public:
    template<class U> requires std::convertible_to <const U&, std::basic_string_view<char>>
    consteval FormatWithSource(U&& inner, std::source_location location = std::source_location::current())
        : m_inner(std::forward<U>(inner))
        , m_source(location)
    {

    }

    constexpr FormatWithSource(const FormatWithSource& other)
        : m_inner(other.m_inner)
        , m_source(other.m_location)
    {

    }

    template<class U>
        requires std::convertible_to <const U&, std::basic_string_view<char>>
    constexpr FormatWithSource& operator=(const FormatWithSource<U>& other)
    {
        m_inner = other.m_inner;
        m_source = other.m_source;
        return *this;
    }
public:
    constexpr Type const& Format() const 
    {
        return m_inner;
    }

    constexpr SourceInfo const& Source() const
    {
        return m_source;
    }

private:
    Type m_inner;
    SourceInfo m_source;
};

class Logger
{
public:
    Logger() = default;

    ~Logger() = default;
public:
    void Log(LogLevel level, const String& message, SourceInfo source = SourceInfo::Current())
    {
        if (level >= m_level)
        {
            LogEntry log;
            log.Time = SystemClock::Now();
            log.Source = source;
            log.Id = Thread::CurrentThreadId();
            log.Level = level;
            log.Message = message;

            // @ 临时
            Console::GetInstance().Write(log.Message);
            // @ 临时
        }
    }

    template<class... Args>
    void Log(LogLevel level, FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        if (level <= m_level)
        {
            LogEntry log;
            log.Time = SystemClock::Now();
            log.Source = withFormat.Source();
            log.Id = Thread::CurrentThreadId();
            log.Level = level;
            log.Message = std::format(withFormat.Format(), std::forward<Args>(args)...);

            // @ 临时
            Console::GetInstance().Write(log.Message);
            // @ 临时
            
        }
    }

    void Verbose(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Verbose, message, source);
    }   

    template<class... Args>
    void Verbose(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Verbose, withFormat, std::forward<Args>(args)...);
    }

    void Debug(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Debug, message, source);
    }  

    template<class... Args>
    void Debug(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Debug, withFormat, std::forward<Args>(args)...);
    }

    void Info(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Info, message, source);
    }

    template<class... Args>
    void Info(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Info, withFormat, std::forward<Args>(args)...);
    }

    void Warning(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Warning, message, source);
    }   

    template<class... Args>
    void Warning(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Warning, withFormat, std::forward<Args>(args)...);
    }

    void Error(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Error, message, source);
    }

    template<class... Args>
    void Error(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Error, withFormat, std::forward<Args>(args)...);
    }

    void Fatal(const String& message, SourceInfo source = SourceInfo::Current())
    {
        Log(LogLevel::Fatal, message, source);
    }

    template<class... Args>
    void Fatal(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Fatal, withFormat, std::forward<Args>(args)...);
    }

    void SetLevel(LogLevel level)
    {
        m_level = level;
    }

    LogLevel GetLevel() const
    {
        return m_level;
    }

    void SetDateTimeKind(DateTimeKind kind)
    {
        m_timeKind = kind;
    }

    DateTimeKind GetDateTimeKind() const
    {
        return m_timeKind;
    }
public:
    static Logger& GetInstance()
    {
        static Logger instance;
        return instance;
    }
private:
    LogLevel m_level = LogLevel::All;
    DateTimeKind m_timeKind = DateTimeKind::Local;
};