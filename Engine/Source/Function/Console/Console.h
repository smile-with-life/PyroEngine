#pragma once

#include "Core.h"
#include "String/String.h"
#include "Color/Color.h"

class Console
{
public:
    Console() = default;

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
};