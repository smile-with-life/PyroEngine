#include "pch.h"

#include "WindowsConsole.h"

Console* Console::Create()
{
    return new WindowsConsole();
}

void WindowsConsole::Init()
{
    // 尝试附加到父进程控制台（若存在）
    if (AttachConsole(ATTACH_PARENT_PROCESS)) 
    {
        m_hwnd = GetConsoleWindow();
        m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        m_isAttached = true;
    }
    else 
    {
        // 附加失败时创建新控制台
        if (!GetConsoleWindow() && AllocConsole()) 
        {
            m_hwnd = GetConsoleWindow();
            m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        else 
        {
            DWORD err = GetLastError();
            // 处理控制台创建失败错误（例如日志记录/返回错误码）
        }
    }

    // 统一重定向标准流
    FILE* fp;
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // Unicode支持
    setlocale(LC_ALL, "chs");

    // 非Debug配置隐藏控制台并重定向到文件
    #if BUILD_CONFIG > BUILD_CONFIG_DEBUG
    Hide();

    HANDLE file = CreateFile(TEXT("consoleLog.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 
    if (file != INVALID_HANDLE_VALUE) 
    {
        // 重定向
        SetStdHandle(STD_OUTPUT_HANDLE, file);
        SetStdHandle(STD_ERROR_HANDLE, file);
        // 获取重定向后的句柄
        m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
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
    return m_isAttached;
}
