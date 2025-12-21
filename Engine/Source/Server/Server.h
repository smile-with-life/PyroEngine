#pragma once
#include "Core.h"
#include "FrameSever/FrameServer.h"
#include "ConsoleServer/ConsoleServer.h"

// ================== 全局变量 ==================​​
inline ConsoleServer* GConsoleServer = nullptr;

inline FrameServer* GFrameServer = nullptr;

inline void AllowServer()
{  
    GConsoleServer = new ConsoleServer();
    GFrameServer = new FrameServer();
}

inline void FreeServer()
{   
    if (!GConsoleServer) 
        delete GConsoleServer;
    if (!GFrameServer)
        delete GFrameServer;
}
