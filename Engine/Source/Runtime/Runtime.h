#pragma once

#include "Core.h"
#include "Console/Console.h"
#include "Logger/Logger.h"
#include "CommandLineArgs/CommandLineArgs.h"
#include "Event/EventSystem.h"
#include "FrameService/FrameService.h"
#include "Window/WindowManager.h"

inline CommandLineArgs* GCommandLineArgs = &CommandLineArgs::GetInstance();
inline Console* GConsole = &Console::GetInstance();
inline EventSystem* GEventSystem = &EventSystem::GetInstance();
inline Logger* GLog = &Logger::GetInstance();
inline FrameService* GFrameService = &FrameService::GetInstance();
inline WindowManager* GWindowManager = &WindowManager::GetInstance();
