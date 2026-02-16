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