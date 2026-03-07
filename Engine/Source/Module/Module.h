#pragma once

#include "Core.h"
#include "Console/Console.h"
#include "Logger/Logger.h"
#include "EventSystem/EventSystem.h"

inline Console* GConsole = nullptr;
inline EventSystem* GEventSystem = nullptr;
inline Logger* GLog = nullptr;


inline void PreInitCoreModule()
{
    GConsole = &Console::GetInstance();
    GEventSystem = &EventSystem::GetInstance();
    GLog = &Logger::GetInstance();
}