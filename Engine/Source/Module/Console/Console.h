#pragma once

#include "Core.h"
#include "String/String.h"


class Console
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~Console() = default;
public:
    /// <summary>
    /// 读取一次输入
    /// </summary>
    /// <returns></returns>
    virtual String Read();

    /// <summary>
    /// 写入内容并换行
    /// </summary>
    /// <param name="content"></param>
    virtual void Write(const String& text);

    /// <summary>
    /// 写入格式化内容
    /// </summary>
    /// <typeparam name="...Args">格式化参数类型</typeparam>
    /// <param name="format">格式字符串</param>
    /// <param name="...args">格式化参数</param>
    template<class... Args>
    void Log(std::format_string<Args...> format, Args &&...args)
    {
        Write(std::format(format, std::forward<Args>(args)...));
    }
public:
    /// <summary>
    /// 获取控制台单例实例
    /// </summary>
    /// <returns></returns>
    static Console& GetInstance();
protected:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Console() = default;
};
