#include "pch.h"
#include "Settings.h"

Settings::~Settings()
{
    Save();
}

Settings::Settings(const String& type, const String& name)
{

}

bool Settings::Load()
{
    return true;
}

void Settings::Save()
{

}

StringList Settings::AllKeys() const
{
    return StringList();
}

void Settings::SetValue(const String& key, const String value)
{

}

String Settings::GetValue(const String& key)
{
    return String();
}

void Settings::Clear()
{

}

bool Settings::Contains(const String& key) const
{
    return true;
}

bool Settings::IsValid() const
{
    return true;
}