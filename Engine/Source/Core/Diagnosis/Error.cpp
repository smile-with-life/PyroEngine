#include "pch.h"

#include "Error.h"

#include "Platform.h"

thread_local char ExceptionErrorBuffer[512] = {};

void EngineAbort(const char* exception)
{
    if (exception) 
    {
        strncpy(ExceptionErrorBuffer, exception, sizeof(ExceptionErrorBuffer) - 1);
        ExceptionErrorBuffer[sizeof(ExceptionErrorBuffer) - 1] = '\0';
    }
    else 
    {
        ExceptionErrorBuffer[0] = '\0';
    }
    Platform::PlatformAbort();
}

