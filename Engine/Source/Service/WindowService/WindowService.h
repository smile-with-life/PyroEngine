#pragma once
#include "Core.h"
#include "String/String.h"
#include "Module/Module.h"
#include "Window/Window.h"
#include "Memory/RAII.h"
#include "Container/Map.h"

class WindowService : public CoreModule
{
public:
    virtual void Init() override
    {

    }

    virtual void Tick() override
    {

    }

    virtual void Exit() override
    {

    }
public:
    bool CreateOSWindow(const String& name)
    {
        if (!m_windows.Contains(name))
        {
            // 创建操作系统原生窗口
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create());

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
            ScopePtr<Window> window = ScopePtr<Window>(Window::Create());

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
private:
    Map<String, ScopePtr<Window>> m_windows;
};