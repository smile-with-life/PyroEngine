#pragma once
#include "Core.h"
#include "String/String.h"

class GUID
{
public:
    GUID();

    ~GUID();

    GUID(const GUID& other) = default;

    GUID& operator=(const GUID& other) = default;

    GUID(GUID&& other) noexcept = default;

    GUID& operator=(GUID&& other) noexcept = default;
public:
    String ToString() const;

    ByteArray ToByteArray() const;

    int32 CompareTo(const GUID& guid) const;

    bool Equal(const GUID& guid) const;

    bool IsEmpty() const;
public:
    friend bool operator==(const GUID& left, const GUID& right);

    friend bool operator!=(const GUID& left, const GUID& right);

    friend bool operator>(const GUID& left, const GUID& right);

    friend bool operator>=(const GUID& left, const GUID& right);

    friend bool operator<(const GUID& left, const GUID& right);

    friend bool operator<=(const GUID& left, const GUID& right);
public:
    static GUID FromString(const String& str);

    static GUID Get();
public:

};