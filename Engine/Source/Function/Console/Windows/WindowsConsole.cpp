#include "pch.h"

#include "WindowsConsole.h"
#include "Platform.h"
#include "String/Convert.h"
#include <io.h>
#include <fcntl.h>
#include <signal.h>

/* static */
Console& Console::GetInstance()
{
    static WindowsConsole instance;
    return instance;
}
/* public */
WindowsConsole::WindowsConsole()
{
    // 附加失败时创建新控制台
    if (AllocConsole()) 
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

    // 获取控制台缓冲区大小
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);
    m_bufferWidth = screenInfo.dwSize.X; 
    m_bufferHeight = screenInfo.dwSize.Y;
    // 可视窗口高度 = 窗口底部 - 窗口顶部 + 1（总行数）
    m_visibleHeight = screenInfo.srWindow.Bottom - screenInfo.srWindow.Top + 1;
    m_visibleWidth = screenInfo.dwSize.X;
    // 输入行固定在可视窗口最后一行
    m_inputRow = screenInfo.srWindow.Bottom;
    _UpdateInputLine();
    // 绘制输入行
    _ClearInputLine();

    Hide();
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
    bool vaildInput = false;

    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);

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
        const KEY_EVENT_RECORD& keyEvent = record.Event.KeyEvent;

        // 仅处理按键事件，其他事件留给IME
        if (record.EventType != KEY_EVENT)
            continue;

        // 仅处理按键按下 + 非空字符
        if (!keyEvent.bKeyDown || keyEvent.uChar.UnicodeChar == 0)
            continue;

        // 更新输入行位置
        _UpdateInputLine();
  
        WORD vkCode = keyEvent.wVirtualKeyCode;
        bool isCtrlPressed = (keyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;

        // 特殊键检测
        SpecialKey key = SpecialKey::None;
        if (isCtrlPressed)
        {
            switch (vkCode)
            {
            case 0x41: key = SpecialKey::CheckAll;  break;
            case 0x43: key = SpecialKey::Copy;      break;
            case 0x56: key = SpecialKey::Paste;     break;
            }
        }
        else
        {
            switch (vkCode)
            {
            
            case VK_UP:    key = SpecialKey::Up;        break;
            case VK_DOWN:  key = SpecialKey::Down;      break;
            case VK_LEFT:  key = SpecialKey::Left;      break;
            case VK_RIGHT: key = SpecialKey::Right;     break;
            case VK_BACK:  key = SpecialKey::Backspace; break;
            case VK_RETURN:key = SpecialKey::Enter;
                vaildInput = true;
                break;
            }
        }

        // 特殊按键处理
        if (key != SpecialKey::None)
        {
            m_specialKeyQueue.push(key);
            switch (key)
            {
            case SpecialKey::Copy:      _CopyKeyHandle();       break;
            case SpecialKey::Paste:     _PasteKeyHandle();      break;
            case SpecialKey::Backspace: _BackspaceKeyHandle();  break;
            case SpecialKey::Enter:     _EnterKeyHandle(text);  break;
            case SpecialKey::Left:      _LeftKeyHandle();       break;
            case SpecialKey::Right:     _RightKeyHandle();      break;
            }
            continue;
        }

        // 处理普通可打印字符
        wchar wch = keyEvent.uChar.UnicodeChar;
        if ((iswprint(static_cast<wint_t>(wch)) || wch == L' ') && m_inputBuffer.size() < 1024)
        {
            int32 length = _GetVisualCharCount(m_inputBuffer);
            _SetCursorPosition(2 + length, m_inputRow);

            DWORD written = 0;
            WriteConsoleW(m_stdoutHandle, &wch, 1, &written, nullptr);
            m_inputBuffer += static_cast<char16>(wch);
        }
    }

    // 光标位置最终校准
    int32 length = _GetVisualCharCount(m_inputBuffer);
    _SetCursorPosition(2 + length, m_inputRow);

    return vaildInput;
}

void WindowsConsole::Write(const String& text)
{

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

void WindowsConsole::SetTitle(const String& title)
{
    std::u16string u16str = Convert::UTF8ToUTF16(title);
    SetConsoleTitleW(reinterpret_cast<const wchar*>(u16str.c_str()));
}

void WindowsConsole::SetFontSize(int32 size)
{
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };
    // 获取当前字体信息
    GetCurrentConsoleFontEx(m_stdoutHandle, FALSE, &font);
    font.dwFontSize.Y = size; // Y轴高度决定字体大小
    font.dwFontSize.X = size / 2; // 宽度自适应
    // 应用新字体设置
    SetCurrentConsoleFontEx(m_stdoutHandle, FALSE, &font);
}

void WindowsConsole::SetTextColor(Color color)
{

}

void WindowsConsole::SetThemeColor(Color color)
{

}

void WindowsConsole::ResetTextColor(Color color)
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
    const DWORD bufferSize = m_bufferWidth * m_bufferHeight;
    COORD coordScreen = { 0, 0 }; // 清空起始位置：左上角
    DWORD count = 0;
    FillConsoleOutputCharacterW(
        m_stdoutHandle,
        L' ',               // 填充字符（宽字符）
        bufferSize,         // 填充数量
        coordScreen,        // 起始坐标
        &count              // 实际填充数量
    );
    _UpdateInputLine();
    _ClearInputLine();
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

void WindowsConsole::Focus()
{
    SetFocus(m_hwnd);
    SetActiveWindow(m_hwnd);
}

bool WindowsConsole::IsVisible() const
{
    return (GetWindowLong(m_hwnd, GWL_STYLE) & WS_VISIBLE) != 0;
}

bool WindowsConsole::IsFocus() const
{
    HWND hFocusWnd = GetFocus();
    return (hFocusWnd == m_hwnd) || (GetAncestor(hFocusWnd, GA_ROOT) == m_hwnd);
}

/* private */
void WindowsConsole::_Copy(const std::u16string& text) const
{
    if (text.empty()) 
        return;

    // 打开剪贴板
    if (!OpenClipboard(m_hwnd))  
        return;
    // 清空剪切板中的数据
    EmptyClipboard();

    // 分配全局内存（剪贴板需使用GMEM_MOVEABLE）
    int64 size = (text.size() + 1) * sizeof(char16);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
    if (hGlobal == nullptr)
    {
        CloseClipboard();
        return;
    }

    // 拷贝内容到全局内存
    wchar* buffer = static_cast<wchar*>(GlobalLock(hGlobal));
    wcscpy_s(buffer, text.size() + 1, reinterpret_cast<const wchar*>(text.c_str()));
    GlobalUnlock(hGlobal);

    // 设置剪贴板数据
    SetClipboardData(CF_UNICODETEXT, hGlobal);
    CloseClipboard();
}

std::u16string WindowsConsole::_Paste() const
{
    std::u16string pasteText;
    if (!OpenClipboard(m_hwnd)) 
        return pasteText;

    // 获取剪贴板中的Unicode文本
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData != nullptr)
    {
        char16* buffer = static_cast<char16*>(GlobalLock(hData));
        if (buffer != nullptr)
        {
            pasteText = buffer;
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();
    return pasteText;
}

void WindowsConsole::_CopyKeyHandle() const
{

}

void WindowsConsole::_PasteKeyHandle()
{
    std::u16string pasteText;
    if (!OpenClipboard(m_hwnd))
        return;

    // 获取剪贴板中的Unicode文本
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData != nullptr)
    {
        char16* buffer = static_cast<char16*>(GlobalLock(hData));
        if (buffer != nullptr)
        {
            pasteText = buffer;
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();

    if (m_inputBuffer.size() + pasteText.size() < 1024)
    {
        int32 length = _GetVisualCharCount(m_inputBuffer);
        _SetCursorPosition(2 + length, m_inputRow);

        DWORD written = 0;
        WriteConsoleW(m_stdoutHandle, reinterpret_cast<const wchar*>(pasteText.data()),
            static_cast<DWORD>(pasteText.size()), &written, nullptr);
        m_inputBuffer += pasteText;
    }
}

void WindowsConsole::_CheckAllKeyHandle() const
{
    
}

void WindowsConsole::_BackspaceKeyHandle()
{
    if (m_inputBuffer.empty()) return;

    // 移除最后一个字符
    char16 lastCh = m_inputBuffer.back();
    m_inputBuffer.pop_back();

    // 计算需要回退的列数（中文2格，英文1格）
    int backCols = (lastCh >= 0x4E00 && lastCh <= 0x9FFF) || (lastCh >= 0x3400 && lastCh <= 0x4DBF) ? 2 : 1;

    // 回退并清除字符
    int32 length = _GetVisualCharCount(m_inputBuffer);
    _SetCursorPosition(2 + length, m_inputRow);

    DWORD written = 0;
    std::wstring backspaceSeq;
    for (int i = 0; i < backCols; ++i)
    {
        backspaceSeq += L'\b';  // 回退光标
        backspaceSeq += L' ';   // 覆盖字符
        backspaceSeq += L'\b';  // 再回退
    }
    WriteConsoleW(m_stdoutHandle, backspaceSeq.data(), static_cast<DWORD>(backspaceSeq.size()), &written, nullptr);
}

void WindowsConsole::_EnterKeyHandle(String& text)
{
    if (!m_inputBuffer.empty()) 
    {

        DWORD written = 0;
        WriteConsoleW(m_stdoutHandle, L"输入: ", 4, &written, nullptr);
        WriteConsoleW(m_stdoutHandle, reinterpret_cast<const wchar*>(m_inputBuffer.data()),
            static_cast<DWORD>(m_inputBuffer.size()), &written, nullptr);
        WriteConsoleW(m_stdoutHandle, L"\n", 1, &written, nullptr);

        text = Convert::UTF16ToUTF8(m_inputBuffer); // 直接赋值
        m_inputBuffer.clear();
        _ClearInputLine();
    }
}

void WindowsConsole::_LeftKeyHandle() const
{

}

void WindowsConsole::_RightKeyHandle() const
{

}
