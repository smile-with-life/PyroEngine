#pragma once

#include "Core.h"
#include "Runtime.h"
#include "Container/Array.h"
#include "Memory/RAII.h"

#include "Display/DisplayView.h"

class DisplaySystem
{
public:
    DisplaySystem() = default;

    ~DisplaySystem() = default;
public:
    void Init()
    {

    }
    
    void Tick()
    {

    }

    void Exit()
    {

    }
public:
    void AddDisplayView(ViewPtr<Window> window)
    {
        m_views.Emplace(window);
    }
private:
    Array<DisplayView> m_views;
};