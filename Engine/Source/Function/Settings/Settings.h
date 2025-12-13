#pragma once
#include "Core.h"
#include "String/String.h"

class Settings
{
public:
    enum class ErrorStatus
    {
        NoError,
        FormatError
    };
public:
    Settings() = delete;

    ~Settings();

    Settings(const String& type, const String& name);
public:
    StringList AllKeys() const;

    bool Contains(const String& key) const;

    void Clear();

    void SetValue(const String& key, const String value);

    String GetValue(const String& key);

    ErrorStatus Status() const;
};