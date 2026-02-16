#pragma once

#include "Core.h"
#include "FrameService/FrameService.h"

// ================== 全局变量 ==================​​
inline FrameService* GFrameService = nullptr;

inline void AllocServer()
{
    GFrameService = new FrameService();
}

inline void FreeServer()
{
    if (!GFrameService) delete GFrameService;
}