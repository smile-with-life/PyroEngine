#pragma once

#include "Core.h"

class Console
{
public:
    Console() = default;

    virtual ~Console() = default;
public:
    // 初始化
    virtual void Init();

    // 帧更新
    virtual void Tick();

    // 退出清理
    virtual void Exit();

    // 隐藏控制台窗口
    virtual void Hide();   

    // 显示控制台窗口
    virtual void Show();

    // 检查窗口可见性
    virtual bool IsVisible() const;
public:
    // 创建平台特定的控制台实例
    static Console* Create();
};