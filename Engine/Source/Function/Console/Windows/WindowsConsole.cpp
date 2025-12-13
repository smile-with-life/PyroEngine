#include "pch.h"

#include "WindowsConsole.h"
#include "Platform.h"
#include "String/Convert.h"
#include <io.h>
#include <fcntl.h>
#include <signal.h>

Console& Console::GetInstance()
{
    static WindowsConsole instance;
    return instance;
}

WindowsConsole::WindowsConsole()
{
    // 尝试附加到父进程控制台（若存在）
    if (AttachConsole(ATTACH_PARENT_PROCESS)) 
    {
        m_hwnd = GetConsoleWindow();
        m_isAttached = true;
    }
    else 
    {
        // 附加失败时创建新控制台
        if (!GetConsoleWindow() && AllocConsole()) 
        {
            m_hwnd = GetConsoleWindow();
            m_isAttached = false;
        }
        else 
        {
            DWORD err = GetLastError();
            OutputDebugStringA("控制台创建失败: ");
            OutputDebugStringA(std::to_string(err).c_str());
            return;
        }
    }
    // 获取标准句柄并验证
    m_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    m_stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    m_stderrHandle = GetStdHandle(STD_ERROR_HANDLE);

    // 检查输入输出句柄
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

    // 设置C运行时locale为UTF-8（Windows 10 1903+支持）
    setlocale(LC_ALL, ".UTF8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // 同步std::cout与C标准输出，解决输出顺序错乱
    std::ios::sync_with_stdio(true);
    std::cout.tie(nullptr);
    // 禁用流缓冲，确保即时输出
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;


    DWORD mode = 0;
    if (GetConsoleMode(m_stdinHandle, &mode))
    {
        // 仅禁用快速编辑，保留行输入/回显/鼠标输入
        mode &= ~ENABLE_QUICK_EDIT_MODE; // 仅保留禁用快速编辑
        mode |= ENABLE_LINE_INPUT;
        mode |= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(m_stdinHandle, mode))
        {
            OutputDebugStringA("控制台输入模式设置失败");
        }
    }

    // 关闭Ctrl+C默认行为
    signal(SIGINT, SIG_IGN);
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    // 设置控制台标题
    SetConsoleTitleW(L"游戏引擎控制台");
}

WindowsConsole::~WindowsConsole()
{
    // 将当前进程从其关联的控制台分离
    FreeConsole();
}

bool WindowsConsole::ReadInput(String& text)
{
    text.Clear();

    INPUT_RECORD inputRecords[32];
    DWORD eventCount = 0;
    bool hasValidInput = false;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_stdoutHandle, &csbi);

    static bool isFirstInput = true;
    if (isFirstInput)
    {
        ClearInputLine();
        RedirectCout();
        isFirstInput = false;
    }

    // 非阻塞检查输入
    if (!PeekConsoleInput(m_stdinHandle, inputRecords, 32, &eventCount) || eventCount == 0)
    {
        return false;
    }

    // 读取事件（不丢弃IME相关事件）
    DWORD readCount = 0;
    if (!ReadConsoleInputW(m_stdinHandle, inputRecords, eventCount, &readCount))
    {
        return false;
    }

    // 处理读取到的事件
    for (DWORD i = 0; i < readCount; ++i)
    {
        const INPUT_RECORD& record = inputRecords[i];

        // 仅处理按键事件，其他事件留给IME
        if (record.EventType != KEY_EVENT)
            continue;

        const KEY_EVENT_RECORD& keyEvent = record.Event.KeyEvent;
        // 仅处理按键按下 + 非空字符
        if (!keyEvent.bKeyDown || keyEvent.uChar.UnicodeChar == 0)
            continue;

        COORD lastLine = GetInputLine();
        SpecialKey key = SpecialKey::None;
        WORD vkCode = keyEvent.wVirtualKeyCode;
        bool isCtrlPressed = (keyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;

        // 特殊键处理（原有逻辑不变）
        if (isCtrlPressed)
        {
            if (vkCode == 0x56) key = SpecialKey::Paste;
            if (vkCode == 0x43) key = SpecialKey::Copy;
            if (vkCode == 0x41) key = SpecialKey::None;
        }
        else
        {
            switch (vkCode)
            {
            case VK_UP:    key = SpecialKey::Up; break;
            case VK_DOWN:  key = SpecialKey::Down; break;
            case VK_LEFT:  key = SpecialKey::Left; break;
            case VK_RIGHT: key = SpecialKey::Right; break;
            case VK_BACK:  key = SpecialKey::Backspace; break;
            case VK_RETURN:key = SpecialKey::Enter; break;
            default: break;
            }
        }

        // 特殊键处理逻辑（原有不变）
        if (key != SpecialKey::None)
        {
            m_specialKeyQueue.push(key);
            switch (key)
            {
            case SpecialKey::Copy:
                CopyToClipboard(m_inputBuffer);
                break;
            case SpecialKey::Paste:
            {
                std::u16string pasteText = PasteFromClipboard();
                if (!pasteText.empty() && m_inputBuffer.size() + pasteText.size() < 1024)
                {
                    int visualLen = GetVisualCharCount(m_inputBuffer);
                    COORD pastePos = { static_cast<SHORT>(2 + visualLen), lastLine.Y };
                    SetConsoleCursorPosition(m_stdoutHandle, pastePos);

                    DWORD written = 0;
                    WriteConsoleW(m_stdoutHandle, reinterpret_cast<const wchar_t*>(pasteText.data()),
                        static_cast<DWORD>(pasteText.size()), &written, nullptr);
                    m_inputBuffer += pasteText;
                }
                break;
            }
            case SpecialKey::Backspace:
                if (!m_inputBuffer.empty()) {
                    m_inputBuffer.pop_back();
                    int visualLen = GetVisualCharCount(m_inputBuffer);
                    COORD backPos = { static_cast<SHORT>(2 + visualLen), lastLine.Y };
                    SetConsoleCursorPosition(m_stdoutHandle, backPos);

                    const wchar_t backspaceSeq[] = { L'\b', L' ', L'\b' };
                    DWORD written = 0;
                    WriteConsoleW(m_stdoutHandle, backspaceSeq, _countof(backspaceSeq) - 1, &written, nullptr);
                }
                break;
            case SpecialKey::Enter:
                if (!m_inputBuffer.empty()) {
                    COORD infoOutputPos = { 0, static_cast<SHORT>(lastLine.Y - 1) };
                    SetConsoleCursorPosition(m_stdoutHandle, infoOutputPos);

                    DWORD written = 0;
                    WriteConsoleW(m_stdoutHandle, L"输入: ", 4, &written, nullptr);
                    WriteConsoleW(m_stdoutHandle, reinterpret_cast<const wchar_t*>(m_inputBuffer.data()),
                        static_cast<DWORD>(m_inputBuffer.size()), &written, nullptr);
                    WriteConsoleW(m_stdoutHandle, L"\n", 1, &written, nullptr);

                    hasValidInput = true;
                    text = Convert::UTF16ToUTF8(m_inputBuffer); // 直接赋值
                    m_inputBuffer.clear();
                    ClearInputLine();
                }
                break;
            default:
                break;
            }
            continue;
        }

        // 处理普通可打印字符（兼容中文）
        wchar_t wch = keyEvent.uChar.UnicodeChar;
        if ((iswprint(static_cast<wint_t>(wch)) || wch == L' ') && m_inputBuffer.size() < 1024)
        {
            int visualLen = GetVisualCharCount(m_inputBuffer);
            COORD charPos = { static_cast<SHORT>(2 + visualLen), lastLine.Y };
            SetConsoleCursorPosition(m_stdoutHandle, charPos);

            DWORD written = 0;
            WriteConsoleW(m_stdoutHandle, &wch, 1, &written, nullptr);
            m_inputBuffer += static_cast<char16>(wch);
            hasValidInput = true;
        }
    }

    // 光标位置最终校准
    int finalVisualLen = GetVisualCharCount(m_inputBuffer);
    COORD finalCursorPos = {
        static_cast<SHORT>(2 + finalVisualLen),
        GetInputLine().Y
    };
    SetConsoleCursorPosition(m_stdoutHandle, finalCursorPos);

    // 非回车时也返回输入内容
    if (!text.IsEmpty() && hasValidInput)
    {
        text = Convert::UTF16ToUTF8(m_inputBuffer);
    }

    return hasValidInput;
}

void WindowsConsole::ReadLine(String& text)
{
    std::string content;
    std::cin >> content;
    text = content;
}

bool WindowsConsole::KeyDetection(SpecialKey& keyCode)
{
    if (m_specialKeyQueue.empty())
    {
        keyCode = SpecialKey::None;
        return false;
    }
    keyCode = m_specialKeyQueue.front();
    m_specialKeyQueue.pop();
    return true;
}

void WindowsConsole::SetOutputColor(Color color)
{

}

void WindowsConsole::SetThemeColor(Color color)
{

}

void WindowsConsole::ResetOutputColor(Color color)
{

}

void WindowsConsole::ResetThemeColor(Color color)
{

}

void WindowsConsole::ResetColor(Color color)
{

}

void WindowsConsole::Clear()
{

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

void WindowsConsole::Active()
{

}

bool WindowsConsole::IsShown() const
{
    return m_hwnd && IsWindowVisible(m_hwnd);
}

bool WindowsConsole::IsAttached() const
{
    return false;
}
