#pragma once
#include "Core.h"
#include "String/String.h"
#include "Time/DateTime.h"

#include <source_location>


class SourceInfo
{
public:
    constexpr SourceInfo() = default;

    constexpr explicit SourceInfo(const std::source_location& location)
        : FileName(location.file_name())
        , FunctionName(location.function_name())
        , Line(location.line())
        , Column(location.column())
    {

    }
public:
    constexpr static SourceInfo Current()
    {
        return SourceInfo(std::source_location::current());
    }
public:
    const char* FileName;
    const char* FunctionName;
    uint32 Line;
    uint32 Column;
};
