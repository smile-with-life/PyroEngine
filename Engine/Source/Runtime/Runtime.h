#pragma once

#include "Core.h"

class CommandLineArgs;
class ConfigManager;
class Console;
class EventSystem;
class Logger;
class FrameService;
class WindowManager;

/* ==================== 无依赖的基础服务 ==================== */
extern CommandLineArgs* const GCommandLineArgs;
extern ConfigManager* const GConfigManager;
extern Console* const GConsole;
extern EventSystem* const GEventSystem;

/* ==================== 有依赖的基础服务 ==================== */
extern Logger* const GLog;
extern FrameService* const GFrameService;
extern WindowManager* const GWindowManager;
