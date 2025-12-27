#pragma once
#include "Core.h"
#include "Container/Array.h"

class ByteArray
{
public:
    ByteArray() = default;

    ~ByteArray() = default;

    ByteArray(const ByteArray& other) = default;

    ByteArray& operator=(const ByteArray& other) = default;

    ByteArray(ByteArray&& other) noexcept = default;

    ByteArray& operator=(ByteArray&& other) noexcept = default;

    ByteArray(const char* data)
    {
        
    }
public:
    void Add(char byte)
    {
        m_data.Push(byte);
    }

    char* Data()
    {
        return m_data.Data();
    }

    const char* Data() const
    {
        return m_data.Data();
    }

    int64 Size() const
    {
        return m_data.Size();
    }

    int64 Max() const
    {

    }

    bool IsEmpty()
    {
        return m_data.IsEmpty();
    }

    void Resize(int64 size)
    {
        m_data.Resize(size);
    }

    void Clear()
    {
        m_data.Clear();
    }

    void Swap(ByteArray& other)
    {
        m_data.Swap(other.m_data);
    }
public:
    char& operator[](int64 index)
    {
        return m_data[index];
    }

    const char& operator[](int64 index) const
    {
        return m_data[index];
    }
private:
    Array<char> m_data;
};