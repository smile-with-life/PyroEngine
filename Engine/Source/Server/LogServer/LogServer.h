#pragma once
#include "Core.h"
#include "Module/Module.h"
#include "String/String.h"
#include "Time/Time.h"
#include "Thread/Thread.h"
#include "Diagnosis/SourceInfo.h"
#include <source_location>

enum class LogLevel : uint16
{
    None,     // 未知
    Verbose,  // 详情：指明程序运行细节
    Debug,    // 调试：指明调试过程中的事件信息
    Info,     // 信息：指明运行过程中的重要信息
    Warning,  // 警告：指明可能潜在的危险状况
    Error,    // 错误：指明错误事件
    Fatal,    // 致命：指明非常严重的可能会导致应用终止执行错误事件
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

struct LogMessage
{
    TimePoint<SystemClock> Time;        // 产生日志消息的时间
    SourceInfo Source;                  // 源代码信息
    ThreadId Id;                        // 产生日志消息的线程ID
    LogLevel Level;                     // 日志消息的级别
    LogLight Light;                     // 日志颜色
    String Message;                     // 日志消息的内容
};

class LogAppender
{

};

class LogServer : public CoreModule
{
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;

    virtual void LoadSettings() override;

    virtual void SaveSettings() override;
private:
    void Log(LogLevel level, const String& message, SourceInfo source = SourceInfo::Current());

    void Log(LogLevel level, LogLight light, const String& message,SourceInfo source = SourceInfo::Current());

    template<class... Args>
    void Log(LogLevel level, WithSourceInfo<std::format_string<Args...>> withFormat, Args &&...args);

    template<class... Args>
    void Log(LogLevel level, LogLight light, WithSourceInfo<std::format_string<Args...>> withFormat, Args &&...args);
};