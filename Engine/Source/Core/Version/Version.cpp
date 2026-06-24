#include "pch.h"

#include "Version.h"

Version Version::Parse(const String& version)
{
    // 解析版本字符串，格式为 "vMajor.Minor.Patch-Stage"  
    return Version();
}

Version::Version(const String& version)
{
    *this = Parse(version);
}

Version::Version(int32 major, int32 minor, int32 patch, VersionStage stage)
    : Major(major), Minor(minor), Patch(patch), Stage(stage)
{

}

Version& Version::IncMajor() 
{
    ++Major;
    Minor = 0;
    Patch = 0;
    Build = 0;
    Stage = VersionStage::None;
    return *this;
}

Version& Version::IncMinor() 
{
    ++Minor;
    Patch = 0;
    Build = 0;
    Stage = VersionStage::None;
    return *this;
}

Version& Version::IncPatch() 
{
    ++Patch;
    Build = 0;
    Stage = VersionStage::None;
    return *this;
}

Version& Version::IncBuild()
{
    ++Build;
    return *this;
}

String Version::ToString() const 
{
    String stage;

    switch (Stage)
    {
    case VersionStage::None :
        stage = "";
        break;
    case VersionStage::Alpha :
        stage = "-alpha";
        break;
    case VersionStage::Beta :
        stage = "-beta";
        break;
    case VersionStage::RC :
        stage = "-rc";
        break;
    case VersionStage::LTS :
        stage = "-lts";
        break;
    }

    return String(std::format("v{}.{}.{}{}", Major, Minor, Patch, stage));
}

bool Version::IsValid() const
{
    return Major >= 0 && Minor >= 0 && Patch >= 0 && Build >= 0;
}

bool Version::IsStable() const
{
    return Stage == VersionStage::LTS;
}

bool operator==(const Version& left, const Version& right)
{
    return left.Major == right.Major
        && left.Minor == right.Minor
        && left.Patch == right.Patch
        && left.Build == right.Build;
}

bool operator!=(const Version& left, const Version& right)
{
    return !(left == right);
}

bool operator<(const Version& left, const Version& right)
{  
    if (left.Major != right.Major)
    {
        return left.Major < right.Major;
    }
    if (left.Minor != right.Minor)
    {
        return left.Minor < right.Minor;
    }
    if (left.Patch != right.Patch)
    {
        return left.Patch < right.Patch;
    }
    if (left.Build != right.Build)
    {
        return left.Build < right.Build;
    }
    return false;
}

bool operator<=(const Version& left, const Version& right)
{
    return !(right < left);
}

bool operator>(const Version& left, const Version& right)
{
    return right < left;
}

bool operator>=(const Version& left, const Version& right)
{
    return !(left < right);
}
