#include "pch.h"

#include "WindowsConsole.h"

void WindowsConsole::Init()
{
    // 获取控制台窗口句柄
    m_hwnd = GetConsoleWindow();
    if (!m_hwnd)
    {
        AllocConsole();// 创建控制台窗口
        m_hwnd = GetConsoleWindow();
    }

    // 获取控制台句柄
    m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (m_handle == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        // 处理错误[2](@ref)
    }

    // 设置控制台窗口标题
    SetConsoleTitle(TEXT("调试控制台"));

    // 支持Unicode输出
    setlocale(LC_ALL, "chs");// 解决中文乱码

    // 重定向标准流
    FILE* fp;
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // 当前构建配置大于 Debug 时，隐藏控制台窗口，将标准输出流重定向到外部文件
    #if BUILD_CONFIG > BUILD_CONFIG_DEBUG
        Hide();
        HANDLE file = CreateFile("consoleLog.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        SetStdHandle(STD_OUTPUT_HANDLE, file);  // 重定向
        HANDLE m_handle = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取重定向后的句柄
    #endif    
}

void WindowsConsole::Tick()
{

}

void WindowsConsole::Exit()
{
    // 将当前进程从其关联的控制台分离
    FreeConsole();
}

void WindowsConsole::Hide()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_HIDE);
    }
}

void WindowsConsole::Show()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_SHOW);
    }
}

bool WindowsConsole::IsVisible() const
{
    return m_hwnd && IsWindowVisible(m_hwnd);
}

bool WindowsConsole::IsAttached() const
{
    return false;
}
