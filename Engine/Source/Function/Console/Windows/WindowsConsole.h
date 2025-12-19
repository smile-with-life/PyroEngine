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
    virtual String Read() override;

    /// <summary>
    /// 写入内容并换行
    /// </summary>
    /// <param name="content"></param>
    virtual void Write(const String& text) override;

    /// <summary>
    /// 非阻塞读取输入
    /// </summary>
    /// <param name="content"></param>
    virtual bool ReadInput(String& text) override;

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

    void _CopyKeyHandle() const;

    void _PasteKeyHandle();

    void _CheckAllKeyHandle() const;

    void _BackspaceKeyHandle();

    void _EnterKeyHandle();

    void _LeftKeyHandle() const;

    void _RightKeyHandle() const;

    void _PrintOutput(const std::u16string& text);

    void _PrintInput(TCHAR wch);

    void _PrintInput(const TCHAR* str);

    void _UpdateInputLine()
    {
        CONSOLE_SCREEN_BUFFER_INFO screenInfo;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);
        m_visibleHeight = screenInfo.srWindow.Bottom - screenInfo.srWindow.Top + 1;
        m_visibleWidth = screenInfo.dwSize.X;

        // 清空输入行所有字符
        DWORD written = 0;
        std::wstring emptyLine(m_bufferWidth, L' ');
        _SetCursorPosition(0, m_inputRow);
        WriteConsoleW(m_stdoutHandle, emptyLine.data(), static_cast<DWORD>(emptyLine.size()), &written, nullptr);

        // 更新输入行位置
        m_inputRow = screenInfo.srWindow.Bottom - 1;

        // 重新绘制>提示符
        _SetCursorPosition(0, m_inputRow);
        WriteConsoleW(m_stdoutHandle, L">", 1, &written, nullptr);
        
        // 光标移到>后方
        _SetCursorPosition(2, m_inputRow);

        if (!m_inputBuffer.empty())
        {
            WriteConsoleW(
                m_stdoutHandle,
                reinterpret_cast<const wchar*>(m_inputBuffer.data()),
                static_cast<DWORD>(m_inputBuffer.size()),
                &written,
                nullptr
            );
        }

    }

    void _CalcOutputLine() const
    {
        CONSOLE_SCREEN_BUFFER_INFO screenInfo;
        GetConsoleScreenBufferInfo(m_stdoutHandle, &screenInfo);

        SMALL_RECT newWindow = screenInfo.srWindow;
        int newTop = newWindow.Top + 1;
        int newBottom = newWindow.Bottom + 1;
        // 检查窗口移动后是否超出缓冲区范围（预留输入行高度）
        bool canMoveWindow = (newBottom + 1) < static_cast<int>(screenInfo.dwSize.Y);

        if (canMoveWindow) 
        {
            // 窗口可移动：直接调整窗口位置
            newWindow.Top = static_cast<SHORT>(newTop);
            newWindow.Bottom = static_cast<SHORT>(newBottom);
            SetConsoleWindowInfo(m_stdoutHandle, TRUE, &newWindow) != 0;
        }
        else 
        {
            // 窗口不可移动：改为滚动内容（等效视口下移）
            SMALL_RECT scrollArea = {
                0,
                0,
                screenInfo.dwSize.X,
                m_inputRow - 1
            };

            // 滚动目标：向上移动一行
            COORD scrollDest = { 0, 0 }; // 滚动目标偏移（相对scrollArea）
            // 填充字符：保持原控制台文本属性（颜色/背景）
            CHAR_INFO fillChar = { L' ', screenInfo.wAttributes };

            // 执行内容滚动（内容上移1行，等效视口下移）
            ScrollConsoleScreenBufferW(
                m_stdoutHandle,
                &scrollArea,    // 要滚动的区域
                nullptr,        // 裁剪区域（nullptr=不裁剪）
                scrollDest,     // 目标偏移
                &fillChar       // 空白区域填充字符
            ) != 0;
        }
    }

    void _SetCursorPosition(int32 x, int32 y) const
    {
        COORD position = { x, y };
        SetConsoleCursorPosition(m_stdoutHandle, position);
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

    std::vector<std::u16string> _SplitText(const std::u16string& text)
    {
        std::vector<std::u16string> lines;
        if (text.empty() || m_visibleWidth <= 0)
        {
            lines.push_back(text);
            return lines;
        }

        // 每行最大可视宽度（控制台宽度-1，留空最后一列）
        const int32 maxLineWidth = m_visibleWidth - 1;
        std::u16string currentLine;
        int32 currentWidth = 0;

        for (char16 ch : text)
        {
            // 遇到换行符：保存当前行，重置
            if (ch == L'\n' || ch == L'\r')
            {
                if (!currentLine.empty())
                {
                    lines.push_back(currentLine);
                    currentLine.clear();
                    currentWidth = 0;
                }
                // 跳过\r（只处理\n作为换行）
                if (ch == L'\r')
                    continue;
                continue;
            }

            // 计算当前字符的视觉宽度
            int32 charWidth = ((ch >= 0x4E00 && ch <= 0x9FFF) || (ch >= 0x3400 && ch <= 0x4DBF)) ? 2 : 1;

            // 如果当前行+当前字符超过最大宽度：保存当前行，新建行
            if (currentWidth + charWidth > maxLineWidth)
            {
                lines.push_back(currentLine);
                currentLine.clear();
                currentWidth = 0;
            }

            // 添加字符到当前行
            currentLine += ch;
            currentWidth += charWidth;
        }

        // 添加最后一行
        if (!currentLine.empty())
        {
            lines.push_back(currentLine);
        }

        return lines;
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