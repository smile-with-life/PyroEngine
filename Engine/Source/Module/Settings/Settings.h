#pragma once
#include "Core.h"
#include "String/String.h"
#include "Container/Map.h"

class Settings
{
public:
    ~Settings();

    Settings(const String& type, const String& name);
public:
    bool Load();

    void Save();

    StringList AllKeys() const;

    void SetValue(const String& key, const String value);

    String GetValue(const String& key);

    void Clear();

    bool Contains(const String& key) const;

    bool IsValid() const;
};