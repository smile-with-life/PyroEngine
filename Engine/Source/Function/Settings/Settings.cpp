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

}

void Settings::Save()
{

}

StringList Settings::AllKeys() const
{

}

void Settings::SetValue(const String& key, const String value)
{

}

String Settings::GetValue(const String& key)
{

}

void Settings::Clear()
{

}

bool Settings::Contains(const String& key) const
{

}

bool Settings::IsValid() const
{

}