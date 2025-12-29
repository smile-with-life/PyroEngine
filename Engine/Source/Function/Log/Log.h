#pragma once
#include "Core.h"
#include "String/String.h"
#include "Time/Time.h"
#include "Thread/Thread.h"
#include "Diagnosis/SourceInfo.h"

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

class Logger
{
public:
    Logger() = default;

    ~Logger() = default;
public:
    void Log(LogLevel level, const String& message, SourceInfo source = SourceInfo::Current());

    template<class... Args>
    void Log(LogLevel level, WithSourceInfo<std::format_string<Args...>> withFormat, Args &&...args);

    void SetLevel(LogLevel level);

    LogLevel GetLevel();
};