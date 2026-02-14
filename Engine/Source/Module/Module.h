#pragma once

#include "Core.h"
#include "Console/Console.h"

inline Console* GConsole = nullptr;


inline void PreInitCoreModule()
{
    GConsole = &Console::GetInstance();
}