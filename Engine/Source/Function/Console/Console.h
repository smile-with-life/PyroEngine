#pragma once

#include "Core.h"
#include "String/String.h"
#include "Color/Color.h"

class Console
{
public:
    Console() = default;

    virtual ~Console() = default;
public:
    /// <summary>
    /// 从标准输入流中读取
    /// </summary>
    /// <param name="text"></param>
    void Read(const String& text);

    /// <summary>
    /// 将指定内容写入标准输出流
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="content"></param>
    template<class Type>
    void Write(const Type& content);

    /// <summary>
    /// 将指定内容按行写入标准输出流
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="content"></param>
    template<class Type>
    void WriteLine(const Type& content);

    /// <summary>
    /// 将指定内容写入标准错误流
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="content"></param>
    template<class Type>
    void WriteError(const Type& content);

    /// <summary>
    /// 将指定内容按行写入标准错误流
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="content"></param>
    template<class Type>
    void WriteErrorLine(const Type& content);

    /// <summary>
    /// 设置控制台输出颜色
    /// </summary>
    /// <param name="color"></param>
    virtual void SetOutputColor(Color color);

    /// <summary>
    /// 设置控制台主题颜色
    /// </summary>
    virtual void SetThemeColor(Color color);

    /// <summary>
    /// 将控制台重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetColor(Color color);

    /// <summary>
    /// 清除控制台缓冲区和相应的显示信息
    /// </summary>
    virtual void Clear();

    /// <summary>
    /// 隐藏控制台窗口
    /// </summary>
    virtual void Hide();   

    /// <summary>
    /// 显示控制台窗口
    /// </summary>
    virtual void Show();

    /// <summary>
    /// 将标准输入重定向到指定路径
    /// </summary>
    virtual void InputRedirect(const String& path);

    /// <summary>
    /// 将标准输出重定向到指定路径
    /// </summary>
    virtual void OutputRedirect(const String& path);

    /// <summary>
    /// 将标准错误重定向到指定路径
    /// </summary>
    virtual void ErrorRedirect(const String& path);

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsVisible() const;

    /// <summary>
    /// 检查控制台是否附加到父进程控制台
    /// </summary>
    /// <returns></returns>
    virtual bool IsAttached() const;
public:
    /// <summary>
    /// 获取控制台单例实例
    /// </summary>
    /// <returns></returns>
    static Console& GetInstance();
};

template<class Type>
inline void Console::Write(const Type& content)
{
    std::cout << content;
}

template<class Type>
inline void Console::WriteLine(const Type& content)
{
    std::cout << content << '\n';
}

template<class Type>
inline void Console::WriteError(const Type& content)
{
    std::cerr << content;
}

template<class Type>
inline void Console::WriteErrorLine(const Type& content)
{
    std::cerr << content << '\n';
}
