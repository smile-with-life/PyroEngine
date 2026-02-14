#include "pch.h"
#include "WindowsConsole.h"

/* static */
Console& Console::GetInstance()
{
    static WindowsConsole instance;
    return instance;
}

/* public */
WindowsConsole::WindowsConsole()
{
    // 创建新控制台窗口
    if (AllocConsole())
    {
        m_hwnd = ::GetConsoleWindow();
    }
    else
    {
        DWORD err = GetLastError();
        OutputDebugStringA("控制台创建失败: ");
        OutputDebugStringA(std::to_string(err).c_str());
        return;
    }
    // 获取标准句柄并验证
    m_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    m_stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    m_stderrHandle = GetStdHandle(STD_ERROR_HANDLE);
    if (m_stdoutHandle == INVALID_HANDLE_VALUE || m_stdinHandle == INVALID_HANDLE_VALUE || m_stderrHandle == INVALID_HANDLE_VALUE)
    {
        OutputDebugStringA("控制台句柄获取失败");
        return;
    }
    // 统一重定向标准流
    FILE* fp;
    freopen_s(&fp, "CONIN$", "rb", stdin);
    freopen_s(&fp, "CONOUT$", "wb", stdout);
    freopen_s(&fp, "CONOUT$", "wb", stderr);
}

WindowsConsole::~WindowsConsole()
{
    FreeConsole();
}

String WindowsConsole::Read()
{
    std::string content;
    std::cin >> content;
    return String(content);
}

void WindowsConsole::Write(const String& text)
{
    std::cout << text << std::endl;
}
