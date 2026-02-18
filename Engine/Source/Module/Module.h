#pragma once

#include "Core.h"
#include "Console/Console.h"
#include "Logger/Logger.h"
#include "EventSystem/EventSystem.h"

inline Console* GConsole = nullptr;
inline Logger* GLog = nullptr;
inline EventSystem* GEventSystem = nullptr;

inline void PreInitCoreModule()
{
    GConsole = &Console::GetInstance();
    GLog = &Logger::GetInstance();
    GEventSystem = &EventSystem::GetInstance();
}