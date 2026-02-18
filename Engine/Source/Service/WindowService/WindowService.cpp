#include "pch.h"

#include "WindowService.h"

void WindowService::Init()
{
    GEventSystem->Subscribe("WindowCloseEvent", MemberFuncBind(OnEvent));
}

void WindowService::Tick()
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

void WindowService::Exit()
{
    GEventSystem->Unsubscribe("WindowCloseEvent", MemberFuncBind(WindowService::OnEvent));
    DestroyAllWindows();
    _ProcessDelayDestroy();
}

bool WindowService::IsSupportDynamicReload()
{
    return false;
}

uint64 WindowService::CreateMainWindow(const WindowProps& props)
{
    if (m_mainWindowId) return 0;

    SharedPtr<Window> mainWindow = SharedPtr<Window>(Window::Create(props));

    m_mainWindowId = mainWindow->GetWindowId();

    m_windows[m_mainWindowId] = mainWindow;

    return m_mainWindowId;
}

uint64 WindowService::CreateSubWindow(const WindowProps& props)
{
    if (!m_mainWindowId) return 0;

    SharedPtr<Window> window = SharedPtr<Window>(Window::Create(props));

    uint64 windowId = window->GetWindowId();

    m_windows[windowId] = window;

    return windowId;
}

bool WindowService::DestroyWindow(uint64 windowId)
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

void WindowService::DestroyAllWindows()
{
    for (const auto& [windowId, _] : m_windows)
    {
        m_delayDestroyIds.Add(windowId);
    }
}

uint64 WindowService::GetMainWindowId() const
{
    return m_mainWindowId;
}

Array<uint64> WindowService::GetAllWindowIds() const
{
    Array<uint64> windowIds;
    for (const auto& [windowId, _] : m_windows)
    {
        windowIds.Add(windowId);
    }
    return windowIds;
}

WeakPtr<Window> WindowService::GetWindow(uint64 windowId)
{
    if (windowId && m_windows.Contains(windowId) &&
        !m_delayDestroyIds.Contains(windowId))
    {
        return m_windows[windowId];
    }
    return WeakPtr<Window>();
}

WeakPtr<Window> WindowService::GetMainWindow()
{
    if (m_mainWindowId && !m_delayDestroyIds.Contains(m_mainWindowId))
    {
        return m_windows[m_mainWindowId];
    }
    return WeakPtr<Window>();
}

bool WindowService::IsWindowValid(uint64 windowId) const
{
    return m_windows.Contains(windowId) && !m_delayDestroyIds.Contains(windowId);
}

int64 WindowService::GetWindowCount() const
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

void WindowService::OnEvent(Event& event)
{
    switch (event.GetType())
    {
    case EventType("WindowCloseEvent") :
        auto& closeEvent = static_cast<WindowCloseEvent&>(event);
        _DelayDestroy(closeEvent.WindowId);
        break;
    }
}

void WindowService::_DelayDestroy(uint64 windowId)
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

void WindowService::_ProcessDelayDestroy()
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
