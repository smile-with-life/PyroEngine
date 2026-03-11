#include "pch.h"

#include "Runtime.h"

#include "CommandLineArgs/CommandLineArgs.h"
#include "ConfigManager/ConfigManager.h"
#include "Console/Console.h"
#include "Event/EventSystem.h"
#include "Logger/Logger.h"
#include "FrameService/FrameService.h"
#include "Window/WindowManager.h"

CommandLineArgs* const GCommandLineArgs = &CommandLineArgs::GetInstance();
ConfigManager* const GConfigManager = &ConfigManager::GetInstance();
Console* const GConsole = &Console::GetInstance();
EventSystem* const GEventSystem = &EventSystem::GetInstance();
Logger* const GLog = &Logger::GetInstance();
FrameService* const GFrameService = &FrameService::GetInstance();
WindowManager* const GWindowManager = &WindowManager::GetInstance();
