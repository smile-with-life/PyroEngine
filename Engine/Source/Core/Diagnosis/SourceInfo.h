#pragma once
#pragma once

#include "Core.h"

#include <source_location>

class SourceInfo
{
public:
    constexpr SourceInfo() = default;

    consteval explicit SourceInfo(const std::source_location& location)
        : FileName(location.file_name())
        , FunctionName(location.function_name())
        , Line(location.line())
        , Column(location.column())
    {

    }
public:
    consteval static SourceInfo Current(std::source_location source = std::source_location::current())
    {
        return SourceInfo(source);
    }
public:
    const char* FileName = "";
    const char* FunctionName = "";
    uint32 Line = 0;
    uint32 Column = 0;
};
