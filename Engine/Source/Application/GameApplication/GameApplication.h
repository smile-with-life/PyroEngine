#pragma once

#include "Core.h"
#include "Runtime.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input/InputSystem.h"
#include "Console/Console.h"
#include "Window/Window.h"
#include "Function/Render/RenderTest.h"

class TestInputHandler : public IGenericInputHandler
{
public:
    TestInputHandler() = default;

    virtual ~TestInputHandler() = default;

    TestInputHandler(uint32 priority)
        : m_priority(priority)
    {

    }
public:
    virtual bool HandleInput(const InputData& input) override
    {
        if (input.Type == InputType::Keyboard)
        {
            GConsole->Log("Input Type: [Keyboard] Input Data: [{}]", (int32)std::get<KeyInput>(input.Data).Code);
        }
        return true;
    }

    virtual uint32 GetPriority() const override
    {
        return m_priority;
    }
private:
    uint32 m_priority = 0;
};

class GameApplication : public Application
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    GameApplication();
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~GameApplication();
public:
    /// <summary>
    /// 初始化
    /// </summary>
    /// <returns></returns>
    virtual int32 Init();
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick();
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit();
    /// <summary>
    /// 创建主窗口
    /// </summary>
    virtual void CreateMainWindow();
    /// <summary>
    /// 事件处理函数
    /// </summary>
    /// <param name="event"></param>
    virtual void OnEvent(Event& event);
    /// <summary>
    /// 处理窗口关闭事件
    /// </summary>
    /// <param name="event"></param>
    virtual void OnAppQuitEvent(AppQuitEvent& event);
private:
    TestInputHandler m_handler = {};

    WindowId m_mainWindowId = 0;
};