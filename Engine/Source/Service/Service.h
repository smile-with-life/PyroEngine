#pragma once
#include "Core.h"
#include "FrameService/FrameService.h"
#include "WindowService/WindowService.h"

// ================== 全局变量 ==================​​
inline FrameService* GFrameService = nullptr;
inline WindowService* GWindowService = nullptr;

inline void AllowServer()
{  
    GFrameService = new FrameService();
    GWindowService = new WindowService();
}

inline void FreeServer()
{   
    if (!GFrameService) delete GFrameService;
    if (!GWindowService) delete GWindowService;
}
