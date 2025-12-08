#pragma once
#include "Core.h"
#include "FrameSever/FrameServer.h"

class FrameServer;// 帧服务
class ConsoleServer;// 控制台服务
class ConfigServer;// 配置服务
class 
// ================== 全局变量 ==================​​
FrameServer* GFrameServer = nullptr;

ConsoleServer* GConsoleServer = nullptr;

ConfigServer* GConfigServer = nullptr;