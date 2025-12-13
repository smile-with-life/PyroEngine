#pragma once

#include "Core.h"
#include "Platform.h"
#include "Console/Console.h"
#include "String/Convert.h"

class WindowsConsole : public Console
{
public:
    WindowsConsole();

    virtual ~WindowsConsole();
public:
    /// <summary>
    /// 从标准输入流中读取一个字节（非阻塞读取）
    /// </summary>
    /// <param name="ch"></param>
    virtual bool ReadInput(String& text) override;

    /// <summary>
    /// 从标准输入流中读取一行（阻塞读取）
    /// </summary>
    /// <param name="ch"></param>
    virtual void ReadLine(String& text) override;

    /// <summary>
    /// 按键检测
    /// </summary>
    /// <param name="keyCode"></param>
    /// <returns></returns>
    bool KeyDetection(SpecialKey& keyCode) override;

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
    /// 将控制台输出颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetOutputColor(Color color) override;

    /// <summary>
    /// 将控制台主题颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetThemeColor(Color color) override;

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
    /// 设置为活动窗口
    /// <span>只有在控制台可见时有效</span>
    /// </summary>
    virtual void Active();

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsShown() const override;

    /// <summary>
    /// 检查控制台是否附加到父进程控制台
    /// </summary>
    /// <returns></returns>
    virtual bool IsAttached() const override;
private:
    void CopyToClipboard(const std::u16string& text)
    {
        if (text.empty()) return;

        // 打开剪贴板
        if (!OpenClipboard(m_hwnd)) return;
        EmptyClipboard();

        // 分配全局内存（剪贴板需使用GMEM_MOVEABLE）
        size_t bufSize = (text.size() + 1) * sizeof(wchar_t);
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, bufSize);
        if (hGlobal == nullptr)
        {
            CloseClipboard();
            return;
        }

        // 拷贝内容到全局内存
        wchar_t* pBuf = static_cast<wchar_t*>(GlobalLock(hGlobal));
        wcscpy_s(pBuf, text.size() + 1, reinterpret_cast<const wchar*>(text.c_str()));
        GlobalUnlock(hGlobal);

        // 设置剪贴板数据
        SetClipboardData(CF_UNICODETEXT, hGlobal);
        CloseClipboard();
    }

    std::u16string PasteFromClipboard()
    {
        std::u16string pasteText;
        if (!OpenClipboard(m_hwnd)) return pasteText;

        // 获取剪贴板中的Unicode文本
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData != nullptr)
        {
            char16* pBuf = static_cast<char16*>(GlobalLock(hData));
            if (pBuf != nullptr)
            {
                pasteText = pBuf;
                GlobalUnlock(hData);
            }
        }

        CloseClipboard();
        return pasteText;
    }

    static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
    {
        if (dwCtrlType == CTRL_C_EVENT)
        {
            return TRUE;
        }
        return FALSE;
    }

    // 获取控制台最后一行位置
    COORD GetConsoleLastLinePos() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(m_stdoutHandle, &csbi)) {
            return { 0, 0 };
        }
        // 最后一行 = 缓冲区高度 - 1（可视区域最后一行）
        return { 0, static_cast<SHORT>(csbi.dwSize.Y - 1) };
    }
    // 清空最后一行并重新绘制>提示符
    void ResetInputLine() {
        COORD lastLine = GetConsoleLastLinePos();
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &csbi);

        // 清空最后一行所有字符（覆盖残留）
        DWORD written = 0;
        std::wstring emptyLine(csbi.dwSize.X, L' ');
        SetConsoleCursorPosition(m_stdoutHandle, lastLine);
        WriteConsoleW(m_stdoutHandle, emptyLine.data(), static_cast<DWORD>(emptyLine.size()), &written, nullptr);

        // 重新绘制>提示符
        SetConsoleCursorPosition(m_stdoutHandle, lastLine);
        WriteConsoleW(m_stdoutHandle, L">", 1, &written, nullptr);

        // 光标移到>后方
        COORD inputPos = { lastLine.X + 1, lastLine.Y };
        SetConsoleCursorPosition(m_stdoutHandle, inputPos);
    }

    void ConsoleOutput(const String& content) {
        std::u16string wContent = Convert::UTF8ToUTF16(content);

        // 2. 获取信息区最后位置（最后一行的上一行）
        COORD lastLine = GetConsoleLastLinePos();
        COORD infoPos = { 0, static_cast<SHORT>(lastLine.Y - 1) };

        // 3. 定位到信息区，输出内容
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &csbi);
        SetConsoleCursorPosition(m_stdoutHandle, infoPos);

        DWORD written = 0;
        WriteConsoleW(m_stdoutHandle, wContent.data(), static_cast<DWORD>(wContent.size()), &written, nullptr);

        // 4. 输出后重置输入行（避免被覆盖）
        ResetInputLine();
    }
    void RedirectCout() {
        class ConsoleBuf : public std::streambuf {
        private:
            WindowsConsole& console;
            std::string buffer;
        public:
            ConsoleBuf(WindowsConsole& c) : console(c) {}
            int overflow(int c) override {
                if (c != EOF) {
                    buffer += static_cast<char>(c);
                }
                else {
                    // 遇到EOF（如std::endl），输出缓冲区内容
                    console.ConsoleOutput(buffer);
                    buffer.clear();
                }
                return c;
            }
            int sync() override {
                console.ConsoleOutput(buffer);
                buffer.clear();
                return 0;
            }
        };

        static ConsoleBuf buf(*this);
        std::cout.rdbuf(&buf);
    }

private:
    // 控制台窗口句柄
    HWND m_hwnd = nullptr;
    // 控制台标准输出句柄
    HANDLE m_stdoutHandle = INVALID_HANDLE_VALUE;
    // 控制台标准输入句柄
    HANDLE m_stdinHandle = INVALID_HANDLE_VALUE;
    // 控制台标准错误句柄
    HANDLE m_stderrHandle = INVALID_HANDLE_VALUE;
    // 是否附加到外部控制台
    bool m_isAttached = false;
    // 是否可见
    bool m_isShown = false;
    // 是否已显示>提示符
    bool m_isShowPrompt = false;
    // 输入缓冲区
    std::u16string m_inputBuffer;
    // 特殊按键队列
    std::queue<SpecialKey> m_specialKeyQueue;
};