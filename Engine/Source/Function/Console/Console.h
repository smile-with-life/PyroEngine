#pragma once

#include "Core.h"
#include "String/String.h"
#include "Color/Color.h"

class Console
{
public:
    // 特殊按键
    enum class SpecialKey 
    { 
        None,           // 未知
        Up,             // 上方向键
        Down,           // 下方向键
        Left,           // 左
        Right,          // 右
        Backspace,      // 退格键
        Enter,          // 回车键
        Copy,           // 复制
        Paste           // 粘贴
    };
public:
    Console() = default;

    virtual ~Console() = default;
public:
    /// <summary>
    /// 从标准输入流中读取（非阻塞读取）
    /// </summary>
    /// <param name="ch"></param>
    virtual bool ReadInput(String& text);

    /// <summary>
    /// 从标准输入流中读取一行（阻塞读取）
    /// </summary>
    /// <param name="ch"></param>
    virtual void ReadLine(String& text);

    /// <summary>
    /// 将指定内容写入标准输出流
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="content"></param>
    template<class Type>
    void Write(const Type& content);

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
    /// 按键检测
    /// </summary>
    /// <param name="keyCode"></param>
    /// <returns></returns>
    virtual bool KeyDetection(SpecialKey& keyCode);    

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
    /// 将控制台输出颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetOutputColor(Color color);

    /// <summary>
    /// 将控制台主题颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetThemeColor(Color color);

    /// <summary>
    /// 将控制台颜色重置为默认值
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
    /// 设置为活动窗口
    /// <span>只有在控制台可见时有效</span>
    /// </summary>
    virtual void Active();

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsShown() const;

    /// <summary>
    /// 是否为活动窗口
    /// </summary>
    /// <returns></returns>
    virtual bool IsActive() const;

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
    std::cout << content << std::endl;
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
