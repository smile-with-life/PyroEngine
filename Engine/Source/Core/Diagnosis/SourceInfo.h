#pragma once
#include "Core.h"
#include "String/String.h"
#include <source_location>


class SourceInfo
{
public:
    explicit SourceInfo(const std::source_location& location)
        : FileName(location.file_name())
        , FunctionName(location.function_name())
        , Line(location.line())
        , Column(location.column())
    {

    }
public:
    static SourceInfo Current()
    {
        return SourceInfo(std::source_location::current());
    }
public:
    const char* FileName;
    const char* FunctionName;
    uint32 Line;
    uint32 Column;
};

/* 包装代码位置信息 */
template<class Type>
class WithSourceInfo {
public:
    constexpr WithSourceInfo(Type&& inner, SourceInfo source = SourceInfo::Current())
        : m_inner(std::forward<Type>(inner))
        , m_source(source)
    {
    }

    constexpr ~WithSourceInfo() = default;

    constexpr WithSourceInfo(const WithSourceInfo& other) = default;

    constexpr operator=(const WithSourceInfo& other) = default;

    constexpr WithSourceInfo(WithSourceInfo&& other) noexcept = default;

    constexpr operator=(WithSourceInfo&& other) noexcept = default;
public:       
    constexpr const Type& Inner() const
    {
        return m_inner;
    }

    constexpr const SourceInfo& Source() const
    {
        return m_source;
    }

private:
    Type m_inner;
    SourceInfo m_source;
};
