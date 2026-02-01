#pragma once
#include "Core.h"
#include "Console/Console.h"
#include "Log/Log.h"

inline Console* GConsole = nullptr;
inline Logger* GLog = nullptr;


inline void PreInitCoreModule()
{
    GConsole = &Console::GetInstance();
    GLog = &Logger::GetInstance();
}