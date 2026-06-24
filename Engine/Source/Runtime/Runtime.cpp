#include "pch.h"

#include "Runtime.h"

#include "CommandLineArgs/CommandLineArgs.h"
#include "Config/ConfigManager.h"
#include "Console/Console.h"
#include "Event/EventSystem.h"
#include "Logger/Logger.h"
#include "FrameService/FrameService.h"
#include "RHI/RHI.h"
#include "Input/InputSystem.h"
#include "Window/WindowManager.h"

/* ==================== 无需初始化的基础服务 ==================== */
CommandLineArgs* const GCommandLineArgs = &CommandLineArgs::GetInstance();
ConfigManager* const GConfigManager = &ConfigManager::GetInstance();
Console* const GConsole = &Console::GetInstance();
EventSystem* const GEventSystem = &EventSystem::GetInstance();
Logger* const GLog = &Logger::GetInstance();

/* ==================== 需要初始化的基础服务 ==================== */
FrameService* const GFrameService = &FrameService::GetInstance();
RHI* const GRHI = &RHI::GetInstance(GraphicsAPI::Vulkan);
InputSystem* const GInputSystem = &InputSystem::GetInstance();
WindowManager* const GWindowManager = &WindowManager::GetInstance();
