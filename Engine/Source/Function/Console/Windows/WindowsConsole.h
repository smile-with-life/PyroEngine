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
    /// 读取一次输入（阻塞）
    /// </summary>
    /// <returns></returns>
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

    void _PrintInput(tchar wch);

    void _PrintInput(const tchar* str);

    void _UpdateInputLine();
    

    void _CalcOutputLine();

    void _SetCursorPosition(int32 x, int32 y);

    int32 _GetVisualCharCount(const std::u16string& utf16Str);

    std::vector<std::u16string> _SplitText(const std::u16string& text)
    {
        std::vector<std::u16string> lines;
        if (text.empty() || m_bufferWidth <= 0)
        {
            lines.push_back(text);
            return lines;
        }

        // 每行最大可视宽度（控制台宽度-1，留空最后一列）
        const int32 maxLineWidth = m_bufferWidth - 1;
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
    // 特殊按键队列
    std::queue<SpecialKey> m_specialKeyQueue;
    // 光标在输入缓冲区中的偏移量
    int32 m_cursor = 0;
    // 缓冲区宽度
    int32 m_bufferWidth = 0;
    // 缓冲区高度
    int32 m_bufferHeight = 0;
    // 输入行
    int32 m_inputRow = 0;
    // 输出行
    int32 m_outputRow = 0;
};