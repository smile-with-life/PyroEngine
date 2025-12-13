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
    COORD GetInputLine() 
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(m_stdoutHandle, &csbi)) {
            return { 0, 0 };
        }
        // 最后一行 = 缓冲区高度 - 1（可视区域最后一行）
        return { 0, static_cast<SHORT>(csbi.dwSize.Y - 1) };
    }
    // 清空最后一行并重新绘制>提示符
    void ClearInputLine() {
        COORD lastLine = GetInputLine();
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

    void RedirectCout()
    {
        // 确保cout使用UTF8编码
        std::cout.imbue(std::locale(".UTF8"));
    }
    // 类内新增：计算UTF-16字符串的视觉字符数（中文/英文都按1个算）
    int GetVisualCharCount(const std::u16string& utf16Str) const
    {
        int visualCount = 0;
        for (char16 ch : utf16Str)
        {
            if (ch < 0x20 || ch == 0x7F) continue;

            bool isChinese = (ch >= 0x4E00 && ch <= 0x9FFF) || (ch >= 0x3400 && ch <= 0x4DBF);
            visualCount += isChinese ? 2 : 1; // 中文占2格，英文占1格
        }
        return visualCount;
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