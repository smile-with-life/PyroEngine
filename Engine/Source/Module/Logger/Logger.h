#pragma once

#include "Core.h"
#include "Time/Time.h"
#include "String/String.h"
#include "Thread/Thread.h"
#include "Diagnosis/SourceInfo.h"

/// <summary>
/// 日志级别
/// </summary>
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

/// <summary>
/// 日志颜色
/// </summary>
enum class LogLight : uint16
{
    None,       // 无颜色（默认）
    Red,        // 红色：通常用于错误和致命错误
    Orange,     // 橙色：通常用于警告
    Yellow,     // 黄色：通常用于警告或重要信息
    Green,      // 绿色：通常用于成功或信息
    Cyan,       // 青色：通常用于调试信息
    Blue,       // 蓝色：通常用于详细信息
    Purple      // 紫色：通常用于特殊标记的信息
};

/// <summary>
/// 日志消息条目
/// </summary>
struct LogMessage
{
    using LogTime = TimePoint<SystemClock>;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    LogMessage() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~LogMessage() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    LogMessage(const LogMessage& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    LogMessage& operator=(const LogMessage& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    LogMessage(LogMessage&& other) noexcept = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    LogMessage& operator=(LogMessage&& other) noexcept = default;
    /// <summary>
    /// 构造函数，指定日志消息的各个属性
    /// </summary>
    /// <param name="time">产生日志消息的时间</param>
    /// <param name="source">源代码信息</param>
    /// <param name="id">产生日志消息的线程 ID</param>
    /// <param name="level">日志消息的级别</param>
    /// <param name="light">日志颜色</param>
    /// <param name="message">日志消息的内容</param>
    LogMessage(LogTime time, SourceInfo source, 
            ThreadId id, LogLevel level, 
            LogLight light, const String& message)
        : Time(time), Source(source)
        , Id(id), Level(level)
        , Light(light), Message(message)
    {

    }

    TimePoint<SystemClock> Time;        // 产生日志消息的时间
    SourceInfo Source;                  // 源代码信息
    ThreadId Id;                        // 产生日志消息的线程 ID
    LogLevel Level;                     // 日志消息的级别
    LogLight Light;                     // 日志颜色
    String Message;                     // 日志消息的内容
};

/// <summary>
/// 包装代码位置信息的格式化字符串模板类
/// </summary>
/// <typeparam name="Type"></typeparam>
template<class Type>
class FormatWithSource
{
public:
    /// <summary>
    /// 构造函数，在编译时捕获源代码位置
    /// </summary>
    /// <param name="inner">格式字符串</param>
    /// <param name="location">源代码位置信息（自动由编译器填充）</param>
    template<class U> requires std::convertible_to <const U&, std::basic_string_view<char>>
    consteval FormatWithSource(U&& format, std::source_location location = std::source_location::current())
        : m_format(std::forward<U>(format))
        , m_source(location)
    {

    }
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    constexpr FormatWithSource(const FormatWithSource& other)
        : m_format(other.m_format)
        , m_source(other.m_location)
    {

    }
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    template<class U>
        requires std::convertible_to <const U&, std::basic_string_view<char>>
    constexpr FormatWithSource& operator=(const FormatWithSource<U>& other)
    {
        m_format = other.m_format;
        m_source = other.m_source;
        return *this;
    }
public:
    /// <summary>
    /// 获取格式字符串
    /// </summary>
    /// <returns>格式字符串的常量引用</returns>
    constexpr Type const& Format() const
    {
        return m_format;
    }
    /// <summary>
    /// 获取源代码位置信息
    /// </summary>
    /// <returns>源代码位置信息的常量引用</returns>
    constexpr SourceInfo const& Source() const
    {
        return m_source;
    }
private:
    /// <summary>
    /// 格式字符串
    /// </summary>
    Type m_format;
    /// <summary>
    /// 源代码位置信息
    /// </summary>
    SourceInfo m_source;
};

/// <summary>
/// 日志记录器
/// </summary>
class Logger
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Logger() = default;
public:
    /// <summary>
    /// 记录指定级别的日志消息
    /// </summary>
    /// <param name="level">日志级别</param>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Log(LogLevel level, const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录详细级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Verbose(const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录调试级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Debug(const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录信息级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Info(const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录警告级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Warning(const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录错误级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Error(const String& message, SourceInfo source = SourceInfo::Current());
    /// <summary>
    /// 记录致命级别日志
    /// </summary>
    /// <param name="message">日志内容</param>
    /// <param name="source">源代码信息（自动捕获）</param>
    void Fatal(const String& message, SourceInfo source = SourceInfo::Current());
public:
    /// <summary>
    /// 记录带格式化的日志消息
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="level">日志级别</param>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Log(LogLevel level, FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        if (level <= m_level)
        {
            LogMessage log(SystemClock::Now(),
                withFormat.Source(),
                Thread::CurrentThreadId(),
                level,
                LogLight::None,
                std::format(withFormat.Format(), std::forward<Args>(args)...));
            _NativeLog(log);
        }
    }
    /// <summary>
    /// 记录带格式化的详细级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Verbose(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Verbose, withFormat, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 记录带格式化的调试级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Debug(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Debug, withFormat, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 记录带格式化的信息级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Info(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Info, withFormat, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 记录带格式化的警告级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Warning(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Warning, withFormat, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 记录带格式化的错误级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Error(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Error, withFormat, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 记录带格式化的致命级别日志
    /// </summary>
    /// <typeparam name="Args">格式化参数类型</typeparam>
    /// <param name="withFormat">包含源代码信息的格式字符串包装器</param>
    /// <param name="args">格式化参数</param>
    template<class... Args>
    void Fatal(FormatWithSource<std::format_string<Args...>> withFormat, Args &&...args)
    {
        Log(LogLevel::Fatal, withFormat, std::forward<Args>(args)...);
    }
public:
    /// <summary>
    /// 设置日志级别过滤器
    /// <para>只有级别高于或等于该值的日志才会被输出。</para>
    /// </summary>
    /// <param name="level">要设置的日志级别</param>
    void SetLevel(LogLevel level);
    /// <summary>
    /// 获取当前日志级别过滤器
    /// </summary>
    /// <returns>当前日志级别</returns>
    LogLevel GetLevel() const;
    /// <summary>
    /// 设置日志时间显示类型
    /// </summary>
    /// <param name="kind">时间类型（UTC 或本地时间）</param>
    void SetTimeKind(TimeKind kind);
    /// <summary>
    /// 获取当前日志时间显示类型
    /// </summary>
    /// <returns>当前时间类型</returns>
    TimeKind GetTimeKind() const;
public:
    /// <summary>
    /// 获取日志记录器单例实例
    /// </summary>
    /// <returns>日志记录器实例的引用</returns>
    static Logger& GetInstance();
private:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Logger() = default;
    /// <summary>
    /// 本地日志输出实现
    /// <para>将日志消息写入实际输出目标（控制台、文件等）。</para>
    /// </summary>
    /// <param name="message">要输出的日志消息</param>
    void _NativeLog(const LogMessage& message);
private:
    /// <summary>
    /// 当前日志级别过滤器
    /// </summary>
    LogLevel m_level = LogLevel::All;
    /// <summary>
    /// 日志时间显示类型
    /// </summary>
    TimeKind m_timeKind = TimeKind::Local;
};