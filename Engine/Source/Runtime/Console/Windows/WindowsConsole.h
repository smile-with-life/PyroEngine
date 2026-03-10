#pragma once

#include "Core.h"
#include "Platform.h"
#include "Console/Console.h"

/// <summary>
/// Windows 平台的控制台实现
/// </summary>
class WindowsConsole : public Console
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    WindowsConsole();
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~WindowsConsole();
public:
    /// <summary>
    /// 读取一次输入（阻塞）
    /// </summary>
    /// <returns></returns>
    virtual String Read() override;
    /// <summary>
    /// 写入内容并换行
    /// </summary>
    /// <param name="content"></param>
    virtual void Write(const String& text) override;
private:
    // 控制台窗口句柄
    HWND m_hwnd = nullptr;
    // 控制台标准输出句柄
    HANDLE m_stdoutHandle = INVALID_HANDLE_VALUE;
    // 控制台标准输入句柄
    HANDLE m_stdinHandle = INVALID_HANDLE_VALUE;
    // 控制台标准错误句柄
    HANDLE m_stderrHandle = INVALID_HANDLE_VALUE;
};