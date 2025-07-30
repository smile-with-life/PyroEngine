#pragma once

#include "Core/Core.h"
#include "Function/Console/Console.h"

class WindowsConsole : public Console
{
public:
    WindowsConsole() = default;

    virtual ~WindowsConsole() = default;
public:
    // 初始化
    virtual void Init();

    // 帧更新
    virtual void Tick();

    // 退出清理
    virtual void Exit();

    // 隐藏控制台窗口
    virtual void Hide();

    // 显示控制台窗口
    virtual void Show();

    // 检查窗口可见性
    virtual bool IsVisible() const;

    // 检查是否附加到外部控制台
    bool IsAttached() const;

    // 将窗口状态（位置，大小）保存到配置文件中
private:
    // 控制台窗口句柄
    HWND m_hwnd = nullptr;
    // 控制台句柄
    HANDLE m_handle = nullptr;
    // 是否附加到外部控制台
    bool m_isAttached = false;
};