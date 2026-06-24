#pragma once

#include "Core.h"
#include "Runtime.h"
#include "Container/Array.h"
#include "Memory/RAII.h"
#include "Window/Window.h"
#include "Event/EventSystem.h"
#include "RHI/RHI.h"
#include "Display/IDisplayContent.h"

class DisplayView
{
public:
    ~DisplayView()
    {
        GEventSystem->Unsubscribe("WindowResizeEvent", MemberFuncBind(OnEvent));
    }

    DisplayView(ViewPtr<Window> window)
        : m_window(window)
    {
        m_swapchain = GRHI->CreateSwapchain(window.RawPtr());

        GEventSystem->Subscribe("WindowResizeEvent", MemberFuncBind(OnEvent));
    }
public:
    void AddDisplayContent(IDisplayContent* content)
    {
        m_contents.Add(content);
    }

    void Render()
    {

    }

    void OnEvent(Event& event)
    {
        switch (event.GetType())
        {
        case EventType("WindowResizeEvent"):
            auto& resizeEvent = static_cast<WindowResizeEvent&>(event);
            m_swapchain->RecreateSwapchain();
            break;
        }
    }
private:
    ViewPtr<Window> m_window;

    RefPtr<RHISwapchain> m_swapchain;

    Array<IDisplayContent*> m_contents;
};
