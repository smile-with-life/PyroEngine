#include "pch.h"

#include "Guid.h"

Guid Guid::New()
{
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local uuids::uuid_random_generator generator(gen);

    Guid Guid;
    Guid.m_uuid = generator();
    return Guid;
}

Guid Guid::Parse(const String& str)
{
    try
    {
        Guid Guid;
        Guid.m_uuid = uuids::uuid::from_string(str.ToStdString()).value();
        return Guid;
    }
    catch (...)
    {
        throw std::invalid_argument("Guid::Parse：字符串格式不是有效的 UUID");
    }
}

bool Guid::IsEmpty() const
{
    return m_uuid.is_nil();
}

String Guid::ToString() const
{
    return uuids::to_string(m_uuid);
}

bool operator==(const Guid& left, const Guid& right)
{
    return left.m_uuid == right.m_uuid;
}

bool operator!=(const Guid& left, const Guid& right)
{
    return !(left == right);
}
