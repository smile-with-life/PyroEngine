#pragma once
#include "Core.h"
#include "Platform.h"
#include "String/String.h"
#include "Module/Module.h"
#include "Window/Window.h"
#include "Memory/RAII.h"
#include "Container/Map.h"
#include "Event/Event.h"

class WindowService : public CoreModule
{
public:
    virtual void Init() override
    {
        EventSystem::GetInstance().Subscribe("WindowCloseEvent", 
            [this](Event& event)
            {
                this->OnWindowClose(static_cast<WindowCloseEvent&>(event));
            });
    }

    virtual void Tick() override
    {
        for (const auto& iter : m_windows)
        {
            iter.second->PumpMessage();
        }
    }

    virtual void Exit() override
    {
        EventSystem::GetInstance().Unsubscribe("WindowCloseEvent");
    }
public:
    bool CreateOSWindow(const String& name)
    {
        if (!m_windows.Contains(name))
        {
            // 创建操作系统原生窗口
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create(name));

            m_windows[name] = std::move(window);
            return true;
        }
        return false;
    }

    bool CreateOSWindow(const String& name, WindowProps props)
    {
        if (!m_windows.Contains(name))
        {
            // 创建操作系统原生窗口
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create(name, props));

            m_windows[name] = std::move(window);
            return true;
        }
        return false; 
    }

    bool DestroyOSWindow(String name)
    {
        auto iter = m_windows.Find(name);
        if (iter != m_windows.end())
        {
            m_windows.Erase(iter);
            return true;
        }
        return false;
    }

    ViewPtr<Window> GetWindowView(String name)
    {
        if (m_windows.Contains(name))
        {
            return ViewPtr<Window>(m_windows[name].RawPtr());
        }
        return ViewPtr<Window>(nullptr);
    }

    int64 GetWindowCount() const
    {
        return m_windows.Size();
    }

    bool IsHasWindow(const String& name)
    {
        return m_windows.Contains(name);
    }

    /*void OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(EventCallback(WindowService::OnWindowClose));
    }*/

    void OnWindowClose(WindowCloseEvent& event)
    {
        DestroyOSWindow(event.Name);
    }
private:
    Map<String, ScopePtr<Window>> m_windows;
};