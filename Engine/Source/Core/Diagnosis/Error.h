#pragma once

#include "Core.h"

extern thread_local char ExceptionErrorBuffer[512];

/// <summary>
/// 引擎异常中止
/// </summary>
[[noreturn]] void EngineAbort(const char* exception);

