#include "pch.h"

#include "WindowManager.h"

#include "Runtime.h"
#include "Event/EventSystem.h"
#include "ConfigManager/ConfigManager.h"

/* ==================== static ==================== */
WindowManager& WindowManager::GetInstance()
{
    static WindowManager instance;
    return instance;
}

/* ==================== public ==================== */
void WindowManager::Init()
{
    GEventSystem->Subscribe("WindowCloseEvent", MemberFuncBind(OnEvent));

    GetConfig();
}

void WindowManager::Tick()
{
    // 先处理待销毁的窗口
    _ProcessDelayDestroy();

    // 处理窗口的消息
    if (m_mainWindowId)
    {
        for (auto& [windowId, window] : m_windows)
        {
            window->PumpMessage();
        }
    }
}

void WindowManager::Exit()
{
    GEventSystem->Unsubscribe("WindowCloseEvent", MemberFuncBind(WindowManager::OnEvent));
    DestroyAllWindows();
    _ProcessDelayDestroy();
}

uint64 WindowManager::CreateMainWindow(const WindowProps& props)
{
    if (m_mainWindowId) return 0;

    SharedPtr<Window> mainWindow = SharedPtr<Window>(Window::Create(props));

    m_mainWindowId = mainWindow->GetWindowId();

    m_windows[m_mainWindowId] = mainWindow;

    return m_mainWindowId;
}

uint64 WindowManager::CreateSubWindow(const WindowProps& props)
{
    if (!m_mainWindowId) return 0;

    SharedPtr<Window> window = SharedPtr<Window>(Window::Create(props));

    uint64 windowId = window->GetWindowId();

    m_windows[windowId] = window;

    return windowId;
}

bool WindowManager::DestroyWindow(WindowId windowId)
{
    if (!windowId || !m_windows.Contains(windowId))
        return false;

    if(!m_delayDestroyIds.Contains(windowId))
    {
        _DelayDestroy(windowId);
        return true;
    }
    return false;
}

void WindowManager::DestroyAllWindows()
{
    for (const auto& [windowId, _] : m_windows)
    {
        m_delayDestroyIds.Add(windowId);
    }
}

uint64 WindowManager::GetMainWindowId() const
{
    return m_mainWindowId;
}

Array<uint64> WindowManager::GetAllWindowIds() const
{
    Array<uint64> windowIds;
    for (const auto& [windowId, _] : m_windows)
    {
        windowIds.Add(windowId);
    }
    return windowIds;
}

WeakPtr<Window> WindowManager::GetWindow(WindowId windowId)
{
    if (windowId && m_windows.Contains(windowId) &&
        !m_delayDestroyIds.Contains(windowId))
    {
        return m_windows[windowId];
    }
    return WeakPtr<Window>();
}

WeakPtr<Window> WindowManager::GetMainWindow()
{
    if (m_mainWindowId && !m_delayDestroyIds.Contains(m_mainWindowId))
    {
        return m_windows[m_mainWindowId];
    }
    return WeakPtr<Window>();
}

bool WindowManager::IsWindowValid(WindowId windowId) const
{
    return m_windows.Contains(windowId) && !m_delayDestroyIds.Contains(windowId);
}

int64 WindowManager::GetWindowCount() const
{
    // 排除正在销毁的窗口
    int64 count = 0;
    for (const auto& [windowId, _] : m_windows)
    {
        if (!m_delayDestroyIds.Contains(windowId))
        {
            count++;
        }
    }
    return count;
}

void WindowManager::OnEvent(Event& event)
{
    switch (event.GetType())
    {
    case EventType("WindowCloseEvent") :
        auto& closeEvent = static_cast<WindowCloseEvent&>(event);
        _DelayDestroy(closeEvent.WindowId);
        break;
    }
}

void WindowManager::GetConfig()
{
    if (GConfigManager->GetValue(".Engine/Engine", "Window"))
    {
        WindowProps props;
        auto config = GConfigManager->GetValue(".Engine/Engine", "Window");
        if (config["Title"]) props.Title = config["Title"].AsString();
        if (config["Width"]) props.Width = config["Width"].AsInt();
        if (config["Height"]) props.Height = config["Height"].AsInt();
        if (config["PositionX"]) props.PositionX = config["PositionX"].AsInt();
        if (config["PositionY"]) props.PositionY = config["PositionY"].AsInt();
        if (config["Opacity"]) props.Opacity = config["Opacity"].AsFloat();
        if (config["IsVSync"]) props.IsVSync = config["IsVSync"].AsBool();
        if (config["IsDisplayTaskbar"]) props.IsDisplayTaskbar = config["IsDisplayTaskbar"].AsBool();
        if (config["IsTopmost"]) props.IsTopmost = config["IsTopmost"].AsBool();
        if (config["IsVisible"]) props.IsVisible = config["IsVisible"].AsBool();
        if (config["IsAcceptInput"]) props.IsAcceptInput = config["IsAcceptInput"].AsBool();
        if (config["IsHasResizeBorder"]) props.IsHasResizeBorder = config["IsHasResizeBorder"].AsBool();
        if (config["IsSupportDragFile"]) props.IsSupportDragFile = config["IsSupportDragFile"].AsBool();
        if (config["IsHasTitlebar"]) props.IsHasTitlebar = config["IsHasTitlebar"].AsBool();
        
        CreateMainWindow(props);
    }
}

/* ==================== private ==================== */
void WindowManager::_DelayDestroy(WindowId windowId)
{
    if (!windowId)
        return;

    // 添加到延迟销毁队列
    if (!m_delayDestroyIds.Contains(windowId))
    {
        m_delayDestroyIds.Add(windowId);
    }

    // 如果是主窗口，销毁所有子窗口
    if (windowId == m_mainWindowId)
    {
        // 将除了主窗口之外的所有窗口都标记为销毁
        for (const auto& [id, window] : m_windows)
        {
            if (id != m_mainWindowId && !m_delayDestroyIds.Contains(id))
            {
                m_delayDestroyIds.Add(id);
            }
        }
    }
}

void WindowManager::_ProcessDelayDestroy()
{
    for (const auto& windowId : m_delayDestroyIds)
    {
        if (windowId == m_mainWindowId)
            m_mainWindowId = 0;
        m_windows.Erase(windowId);
    }

    m_delayDestroyIds.Clear();

    if (!m_mainWindowId)
    {
        AppQuitEvent event;
        GEventSystem->Publish(event);
    }
}
