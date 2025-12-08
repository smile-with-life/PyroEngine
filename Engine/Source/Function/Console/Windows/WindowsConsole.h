#pragma once

#include "Core.h"
#include "Platform.h"
#include "Console/Console.h"

class WindowsConsole : public Console
{
public:
    WindowsConsole();

    virtual ~WindowsConsole();
public:
    /// <summary>
    /// 设置控制台输出颜色
    /// </summary>
    /// <param name="color"></param>
    virtual void SetOutputColor(Color color) override;

    /// <summary>
    /// 设置控制台主题颜色
    /// </summary>
    virtual void SetThemeColor(Color color) override;

    /// <summary>
    /// 将控制台重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetColor(Color color) override;

    /// <summary>
    /// 清除控制台缓冲区和相应的显示信息
    /// </summary>
    virtual void Clear() override;

    /// <summary>
    /// 隐藏控制台窗口
    /// </summary>
    virtual void Hide() override;

    /// <summary>
    /// 显示控制台窗口
    /// </summary>
    virtual void Show() override;

    /// <summary>
    /// 将标准输入重定向到指定路径
    /// </summary>
    virtual void InputRedirect(const String& path) override;

    /// <summary>
    /// 将标准输出重定向到指定路径
    /// </summary>
    virtual void OutputRedirect(const String& path) override;

    /// <summary>
    /// 将标准错误重定向到指定路径
    /// </summary>
    virtual void ErrorRedirect(const String& path) override;

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsVisible() const override;

    /// <summary>
    /// 检查控制台是否附加到父进程控制台
    /// </summary>
    /// <returns></returns>
    virtual bool IsAttached() const override;
private:
    // 控制台窗口句柄
    HWND m_hwnd = nullptr;
    // 控制台句柄
    HANDLE m_handle = nullptr;
    // 是否附加到外部控制台
    bool m_isAttached = false;
};