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
        CONSOLE_SCREEN_BUFFER_INFO screenInfo;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);
        m_visibleHeight = screenInfo.srWindow.Bottom - screenInfo.srWindow.Top + 1;
        m_visibleWidth = screenInfo.dwSize.X;

        // 强制输入行 = 可视窗口最后一行（锁定）
        m_inputRow = screenInfo.srWindow.Bottom;

        // 更新缓冲区尺寸
        m_bufferWidth = screenInfo.dwSize.X;
        m_bufferHeight = screenInfo.dwSize.Y;
    }
    void _UpdateOutputLine() const
    {
        CONSOLE_SCREEN_BUFFER_INFO screenInfo;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);

        // 输出区最后一行 = 输入行 - 1（输入行上一行）
        SHORT outputRow = m_inputRow - 1;
        // 输出区第一行 = 可视窗口顶部
        SHORT outputFirstRow = screenInfo.srWindow.Top;

        // 滚动触发条件：输出行 < 输出区第一行（输出填满输出区）
        if (outputRow < outputFirstRow)
        {
            // 定义滚动区域：整个输出区（除输入行外的所有可视行）
            SMALL_RECT scrollRect = {
                0,                  // 左列
                outputFirstRow,     // 上行（输出区第一行）
                m_visibleWindowWidth - 1, // 右列
                m_fixedInputRow - 1 // 下行（输入行上一行）
            };
            // 滚动目标：向上移动1行（覆盖最顶行）
            COORD destCoord = { 0, outputFirstRow - 1 };
            // 填充空行的字符（保持背景色）
            CHAR_INFO fillChar = { L' ', screenInfo.wAttributes };

            // 执行滚动：输出区整体上移1行，腾出最后一行作为新输出行
            ScrollConsoleScreenBufferW(
                m_stdoutHandle,
                &scrollRect,    // 要滚动的区域
                nullptr,        // 裁剪区域（null = 整个区域）
                destCoord,      // 滚动后的目标位置
                &fillChar       // 填充空出的行
            );

            // 滚动后，新的输出行 = 输入行上一行（原输出区最后一行）
            outputRow = m_fixedInputRow - 1;
        }

        // 返回输出行坐标（第一列，计算后的输出行）
        
    }
    // 清空输入行
    void _ClearInputLine() 
    {
        // 清空最后一行所有字符（覆盖残留）
        DWORD written = 0;
        std::wstring emptyLine(m_bufferWidth, L' ');
        _SetCursorPosition(0, m_inputRow);
        WriteConsoleW(m_stdoutHandle, emptyLine.data(), static_cast<DWORD>(emptyLine.size()), &written, nullptr);

        // 重新绘制>提示符
        _SetCursorPosition(0, m_inputRow);
        WriteConsoleW(m_stdoutHandle, L">", 1, &written, nullptr);

        // 光标移到>后方
        _SetCursorPosition(1, m_inputRow);
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
        int32 visualCount = 0;
        for (char16 ch : utf16Str)
        {
            if (ch < 0x20 || ch == 0x7F) continue;

            // 扩展中日韩字符 + 全角符号
            bool isWideChar = (ch >= 0x4E00 && ch <= 0x9FFF) ||
                (ch >= 0x3400 && ch <= 0x4DBF) ||
                (ch >= 0xFF01 && ch <= 0xFF60) ||
                (ch >= 0xFFE0 && ch <= 0xFFE6);
            visualCount += isWideChar ? 2 : 1;
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
    // 特殊按键队列
    std::queue<SpecialKey> m_specialKeyQueue;
    // 光标在输入缓冲区中的偏移量
    int32 m_cursor = 0;
    // 缓冲区宽度
    int32 m_bufferWidth = 0;
    // 缓冲区高度
    int32 m_bufferHeight = 0;

    int32 m_visibleWidth = 0;
    int32 m_visibleHeight = 0;
    int32 m_inputRow = 0;
    int32 m_outputRow = 0;
};