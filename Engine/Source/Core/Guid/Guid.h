#pragma once

#include "Core.h"
#include "String/String.h"

#include <stduuid.h>

class Guid
{
public:
    Guid() = default;

    ~Guid() = default;

    Guid(const Guid& other) = default;

    Guid& operator=(const Guid& other) = default;

    Guid(Guid&& other) noexcept = default;

    Guid& operator=(Guid&& other) noexcept = default;
public:
    bool IsEmpty() const;

    String ToString() const;
public:
    friend bool operator==(const Guid& left, const Guid& right);

    friend bool operator!=(const Guid& left, const Guid& right);
public:
    static Guid New();

    static Guid Parse(const String& str);
private:
    uuids::uuid m_uuid;
};