#pragma once
#include "Core.h"
#include "Platform.h"
#include "Service.h"
#include "Container/Map.h"
#include "String/String.h"
#include "Memory/RAII.h"
#include "Window/Window.h"
#include "Event/Event.h"

class WindowService : public CoreService
{
public:
    virtual void Init() override
    {
        m_mainWindow = ScopePtr<Window>(Window::Create("EngineWindow"));
        EventSystem::GetInstance().Subscribe("WindowCloseEvent", 
            [this](Event& event)
            {
                this->OnWindowClose(static_cast<WindowCloseEvent&>(event));
            });
    }

    virtual void Tick() override
    {
        m_mainWindow->PumpMessage();
        for (const auto& value : m_subWindows)
        {
            value.second->PumpMessage();
        }

        _ProcessDelayDeletion();
    }

    virtual void Exit() override
    {
        EventSystem::GetInstance().Unsubscribe("WindowCloseEvent");
    }
public:
    bool CreateSubWindow(const String& name)
    {
        if (!m_subWindows.Contains(name))
        {
            // 创建操作系统原生窗口
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create(name));

            m_subWindows[name] = std::move(window);
            return true;
        }
        return false;
    }

    bool CreateSubWindow(const String& name, WindowProps props)
    {
        if (!m_subWindows.Contains(name))
        {
            // 创建操作系统原生窗口
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create(name, props));

            m_subWindows[name] = std::move(window);
            return true;
        }
        return false; 
    }

    bool DestroySubWindow(const String& name)
    {
        if (m_subWindows.Contains(name))
        {
            _DelayDeletion(name);
            return true;
        }
        return false;
    }

    Window* GetWindowView(const String& name)
    {
        if (!m_delayDeletion.Contains(name))
        {
            if (m_subWindows.Contains(name))
            {
                return m_subWindows[name].RawPtr();
            }
        }
        return nullptr;
    }

    int64 GetSubWindowCount() const
    {
        return m_subWindows.Size();
    }

    bool IsHasSubWindow(const String& name)
    {
        return m_subWindows.Contains(name);
    }

    void OnWindowClose(WindowCloseEvent& event)
    {
        DestroySubWindow(event.Name);
    }
private:
    void _DelayDeletion(const String& name)
    {
        m_delayDeletion.Push(name);
    }

    void _ProcessDelayDeletion()
    {
        for (const auto& name : m_delayDeletion)
        {
            auto iter = m_subWindows.Find(name);
            if (iter != m_subWindows.end())
            {
                m_subWindows.Erase(iter);
            }
        }
    }
private:
    ScopePtr<Window> m_mainWindow;
    Map<String, ScopePtr<Window>> m_subWindows;
    Array<String> m_delayDeletion;
};