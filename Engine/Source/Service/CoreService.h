#pragma once

#include "Core.h"
#include "FrameService/FrameService.h"

// ================== 全局变量 ==================​​
inline FrameService* GFrameService = nullptr;

inline void AllocCoreServer()
{
    GFrameService = new FrameService();
}

inline void FreeCoreServer()
{
    if (!GFrameService) delete GFrameService;
}