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
        Left,           // 左方向键
        Right,          // 右方向键
        Backspace,      // 退格键
        Enter,          // 回车键
        Copy,           // 复制
        Paste,          // 粘贴
        CheckAll        // 全选
    };
public:
    Console() = default;

    virtual ~Console() = default;
public:
    virtual String Read();

    /// <summary>
    /// 写入内容并换行
    /// </summary>
    /// <param name="content"></param>
    virtual void Write(const String& text);

    /// <summary>
    /// 非阻塞读取输入
    /// </summary>
    /// <param name="content"></param>
    virtual bool ReadInput(String& text);

    /// <summary>
    /// 按键检测
    /// </summary>
    /// <param name="keyCode"></param>
    /// <returns></returns>
    virtual bool KeyDetection(SpecialKey& keyCode); 

    /// <summary>
    /// 设置控制台标题
    /// </summary>
    /// <param name="title"></param>
    virtual void SetTitle(const String& title);

    /// <summary>
    /// 设置控制台字体大小
    /// </summary>
    /// <param name="size"></param>
    virtual void SetFontSize(int32 size);

    /// <summary>
    /// 设置控制台文本输出颜色
    /// </summary>
    /// <param name="color"></param>
    virtual void SetTextColor(Color color);

    /// <summary>
    /// 设置控制台主题背景颜色
    /// </summary>
    virtual void SetThemeColor(Color color);

    /// <summary>
    /// 将控制台文本输出颜色重置为默认值
    /// </summary>
    /// <param name="color"></param>
    virtual void ResetTextColor(Color color);

    /// <summary>
    /// 将控制台主题背景颜色重置为默认值
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
    /// 获取输入焦点
    /// <span>只有在控制台可见时有效</span>
    /// </summary>
    virtual void Focus();

    /// <summary>
    /// 检查窗口可见性
    /// </summary>
    /// <returns></returns>
    virtual bool IsVisible() const;

    /// <summary>
    /// 是否获取输入焦点
    /// </summary>
    /// <returns></returns>
    virtual bool IsFocus() const;
public:
    /// <summary>
    /// 获取控制台单例实例
    /// </summary>
    /// <returns></returns>
    static Console& GetInstance();
};