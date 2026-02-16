#include "pch.h"

#include "Logger.h"

/* static */
Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

/* public */
void Logger::Log(LogLevel level, const String& message, SourceInfo source)
{
    if (level >= m_level)
    {
        LogMessage log(SystemClock::Now(), 
            source, 
            Thread::CurrentThreadId(),
            level, 
            LogLight::None,
            message);
        _NativeLog(log);
    }
}

void Logger::Verbose(const String& message, SourceInfo source)
{
    Log(LogLevel::Verbose, message, source);
}

void Logger::Debug(const String& message, SourceInfo source)
{
    Log(LogLevel::Debug, message, source);
}

void Logger::Info(const String& message, SourceInfo source)
{
    Log(LogLevel::Info, message, source);
}

void Logger::Warning(const String& message, SourceInfo source)
{
    Log(LogLevel::Warning, message, source);
}

void Logger::Error(const String& message, SourceInfo source)
{
    Log(LogLevel::Error, message, source);
}

void Logger::Fatal(const String& message, SourceInfo source)
{
    Log(LogLevel::Fatal, message, source);
}

void Logger::SetLevel(LogLevel level)
{
    m_level = level;
}

LogLevel Logger::GetLevel() const
{
    return m_level;
}

void Logger::SetTimeKind(TimeKind kind)
{
    m_timeKind = kind;
}

TimeKind Logger::GetTimeKind() const
{
    return m_timeKind;
}

/* private */
void Logger::_NativeLog(const LogMessage& message)
{
    std::cout << "[" << SystemClock::ToTime(message.Time) << "] "
        << "[" << message.Source.FileName << ":" << message.Source.Line << " " << message.Source.FunctionName << "] "
        << "[" << message.Id << "] "
        << "[" << static_cast<uint16>(message.Level) << "] "
        << message.Message
        << std::endl;
}