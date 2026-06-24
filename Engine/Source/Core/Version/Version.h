#pragma once

#include "Core.h"
#include "String/String.h"

enum class VersionStage
{
    None,   // 无阶段，表示开发版本，适合生产环境使用
    Alpha,  // 预览版本，可能包含不稳定的功能和接口，适合测试和反馈
    Beta,   // 测试版本，功能较为完整，但可能仍存在一些问题，适合更广泛的测试和反馈
    RC,     // 发布候选版本，功能完整且经过测试，准备发布正式版本，适合最后阶段的测试和反馈
    LTS     // 长期支持版本，功能稳定且经过充分测试，适合生产环境使用，提供长期的维护和支持
};

class Version 
{
public:
    Version() = default;

    ~Version() = default;

    Version(const Version& other) = default;

    Version& operator=(const Version& other) = default; 

    Version(Version&& other) noexcept = default;

    Version& operator=(Version&& other) noexcept = default;

    explicit Version(const String& version);  // 从字符串解析

    Version(int32 major, int32 minor, int32 patch, VersionStage stage);
public:
    Version& IncMajor(); 

    Version& IncMinor();

    Version& IncPatch();

    Version& IncBuild();

    String ToString() const;

    bool IsValid() const;

    bool IsStable() const;
public:
    friend bool operator==(const Version& left, const Version& right);

    friend bool operator!=(const Version& left, const Version& right);

    friend bool operator<(const Version& left, const Version& right);

    friend bool operator<=(const Version& left, const Version& right);

    friend bool operator>(const Version& left, const Version& right);

    friend bool operator>=(const Version& left, const Version& right);
public:
    static Version Parse(const String& version);
public:
    int32 Major = 0;
    int32 Minor = 0;
    int32 Patch = 0;
    int32 Build = 0;
    VersionStage Stage = VersionStage::None;
};
