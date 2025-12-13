#pragma once
#include "Core.h"
#include "FrameSever/FrameServer.h"
#include "ConsoleServer/ConsoleServer.h"

// ================== 全局变量 ==================​​
inline FrameServer* GFrameServer = nullptr;

inline ConsoleServer* GConsoleServer = new ConsoleServer();
