#include "pch.h"

#include "WindowService.h"

void WindowService::Init()
{

}

void WindowService::Tick()
{
    // 先处理待销毁的窗口
    _ProcessDelayDestroy();

    // 处理窗口的消息
    if (m_mainWindow)
    {
        m_mainWindow->PumpMessage();
        for (auto& [name, window] : m_subWindows)
        {
            window->PumpMessage();
        }
    }
}

void WindowService::Exit()
{
    DestroyAllWindows();
}

bool WindowService::IsSupportDynamicReload()
{
    return false;
}

bool WindowService::CreateMainWindow(const WindowProps& props)
{
    if (!m_mainWindow && !m_delayDestory.Contains("Main"))
    {
        m_mainWindow = ScopePtr<Window>(Window::Create(props));
        return true;
    }
    return false;
}

bool WindowService::DestroyMainWindow()
{
    if(m_mainWindow)
    {
        _DelayDestroy("Main");
        return true;
    }
    return false;
}

bool WindowService::CreateSubWindow(const String& name, const WindowProps& props)
{
    if (name.IsEmpty()) 
        return false;

    if (m_subWindows.Contains(name))
        return false;

    // 创建子窗口
    auto window = ScopePtr<Window>(Window::Create(props));

    // 添加到管理
    m_subWindows[name] = std::move(window);

    return true;
}

bool WindowService::DestroySubWindow(const String& name)
{
    if (name.IsEmpty())
        return false;

    if (m_subWindows.Contains(name))
    {
        _DelayDestroy(name);
        return true;
    }

    return false;
}

const Window* WindowService::GetWindow(const String& name)
{
    if (!m_delayDestory.Contains(name))
    {
        if (name == "Main")
        {
            return m_mainWindow.RawPtr();
        }
        else
        {
            if (m_subWindows.Contains(name))
            {
                return m_subWindows[name].RawPtr();
            }
        }
    }

    return nullptr;
}

int64 WindowService::GetWindowCount() const
{
    int64 count = 0;

    // 计算有效的主窗口
    if (m_mainWindow && !m_delayDestory.Contains("Main"))
    {
        count++;
    }

    // 计算有效的子窗口
    for (const auto& [name, window] : m_subWindows)
    {
        if (!m_delayDestory.Contains(name))
        {
            count++;
        }
    }

    return count;
}

void WindowService::DestroyAllWindows()
{
    for (const auto& [subName, _] : m_subWindows)
    {
        m_delayDestory.Add(subName);
    }
    m_delayDestory.Add("Main");
}

void WindowService::_DelayDestroy(const String& name)
{
    if (name == "Main")
    {
        for (const auto& [subName, _] : m_subWindows)
        {
            m_delayDestory.Add(subName);
        }
        m_delayDestory.Add(name);
    }
    else
    {
        m_delayDestory.Add(name);
    }
}

void WindowService::_ProcessDelayDestroy()
{
    for (const auto& name : m_delayDestory)
    {
        if (name == "Main")
        {
            m_mainWindow = nullptr;
        }
        else
        {
            m_subWindows.Erase(name);
        }
    }
    m_delayDestory.Clear();
}
