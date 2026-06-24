#pragma once

#include "Core.h"

/* ==================== 无需初始化的基础服务 ==================== */
class CommandLineArgs;
class ConfigManager;
class Console;
class EventSystem;
class Logger;

extern CommandLineArgs* const GCommandLineArgs;
extern ConfigManager* const GConfigManager;
extern Console* const GConsole;
extern EventSystem* const GEventSystem;
extern Logger* const GLog;


/* ==================== 需要初始化的基础服务 ==================== */

class FrameService;
class RHI;
class InputSystem;
class WindowManager;
class AssetManager;

extern FrameService* const GFrameService;
extern RHI* const GRHI;
extern InputSystem* const GInputSystem;
extern WindowManager* const GWindowManager;
extern AssetManager* const GAssetManager;
