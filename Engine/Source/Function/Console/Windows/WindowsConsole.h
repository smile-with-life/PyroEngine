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
    /// 非阻塞读取输入
    /// </summary>
    /// <param name="content"></param>
    virtual bool ReadInput(String& text) override;

    /// <summary>
    /// 写入内容并换行
    /// </summary>
    /// <param name="content"></param>
    virtual void Write(const String& text) override;

    /// <summary>
    /// 按键检测
    /// </summary>
    /// <param name="keyCode"></param>
    /// <returns></returns>
    virtual bool KeyDetection(SpecialKey& keyCode) override;

    /// <summary>
    /// 设置控制台标题
    /// </summary>
    /// <param name="title"></param>
    virtual void SetTitle(const String& title) override;

    /// <summary>
    /// 设置控制台字体大小
    /// </summary>
    /// <param name="size"></param>
    virtual void SetFontSize(int32 size);

    /// <summary>
    /// 设置控制台文本输出颜色
    /// </summary>
    /// <param name="color"></param>
    virtual void SetTextColor(Color color) override;

    /// <summary>
    /// 设置控制台主题背景颜色
    /// </summary>
    virtual void SetThemeColor(Color color) override;

    /// <summary>
    /// 将控制台文本输出颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetTextColor(Color color) override;

    /// <summary>
    /// 将控制台主题背景颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetThemeColor(Color color) override;

    /// <summary>
    /// 将控制台颜色重置为默认值
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
    /// 获取输入焦点
    /// <span>只有在控制台可见时有效</span>
    /// </summary>
    virtual void Focus() override;

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsVisible() const override;

    /// <summary>
    /// 是否获取输入焦点
    /// </summary>
    /// <returns></returns>
    virtual bool IsFocus() const override;
private:
    void _Copy(const std::u16string& text) const;

    std::u16string _Paste() const;

    void _CopyKeyHandle() const;

    void _PasteKeyHandle();

    void _CheckAllKeyHandle() const;

    void _BackspaceKeyHandle();

    void _EnterKeyHandle(String& text);

    void _LeftKeyHandle() const;

    void _RightKeyHandle() const;

    static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
    {
        if (dwCtrlType == CTRL_C_EVENT)
        {
            return TRUE;
        }
        return FALSE;
    }

    // 获取控制台窗口的最后一行位置
    void _UpdateInputLine()
    {
        // 更新控制台窗口信息
        GetConsoleScreenBufferInfo(m_stdoutHandle, &m_screenInfo);
        // 计算可视窗口最后一行的行号
        // srWindow: 可视窗口在缓冲区中的坐标（Top=起始行，Bottom=结束行）
        SHORT row = m_screenInfo.srWindow.Bottom;
        // 若窗口未初始化，使用缓冲区最后一行
        if (row <= 0)
        {
            row = m_screenInfo.dwSize.Y - 1;
        }
        
        m_line = { 0, row };
    }
    // 清空输入行
    void _ClearInputLine() 
    {
        // 清空最后一行所有字符（覆盖残留）
        DWORD written = 0;
        std::wstring emptyLine(1024, L' ');
        _SetCursorPosition(m_line.X, m_line.Y);
        WriteConsoleW(m_stdoutHandle, emptyLine.data(), static_cast<DWORD>(emptyLine.size()), &written, nullptr);

        // 重新绘制>提示符
        _SetCursorPosition(m_line.X, m_line.Y);
        WriteConsoleW(m_stdoutHandle, L">", 1, &written, nullptr);

        // 光标移到>后方
        _SetCursorPosition(m_line.X + 1, m_line.Y);
    }

    void _SetCursorPosition(int32 x, int32 y) const
    {
        COORD position = { x, y };
        SetConsoleCursorPosition(m_stdoutHandle, position);
    }

    void RedirectCout()
    {
        // 确保cout使用UTF8编码
        std::cout.imbue(std::locale(".UTF8"));
    }
    // 类内新增：计算UTF-16字符串的视觉字符数 中文占2格，英文占1格
    int32 _GetVisualCharCount(const std::u16string& utf16Str) const
    {
        int visualCount = 0;
        for (char16 ch : utf16Str)
        {
            if (ch < 0x20 || ch == 0x7F) continue;

            bool isChinese = (ch >= 0x4E00 && ch <= 0x9FFF) || (ch >= 0x3400 && ch <= 0x4DBF);
            visualCount += isChinese ? 2 : 1; 
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
    // 输入行信息
    COORD m_line;
    // 控制台窗口信息
    CONSOLE_SCREEN_BUFFER_INFO m_screenInfo;
    // 特殊按键队列
    std::queue<SpecialKey> m_specialKeyQueue;
    // 光标在输入缓冲区中的偏移量
    int32 m_cursor = 0;
};