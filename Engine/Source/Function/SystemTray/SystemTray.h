#pragma once
#include "Core.h"
#include "Container/Array.h"

/// <summary>
/// 系统托盘图标
/// </summary>
class TrayIcon
{
public:
    void SetToolTip();

    void SetContextMenu();

    void SetIcon();
};

/// <summary>
/// 系统托盘
/// </summary>
class SystemTray
{
public:
    static void AddTrayIcon(const TrayIcon& item);

    static Array<TrayIcon> GetTrayIcons();
public:
    static SystemTray& GetInstance();
};