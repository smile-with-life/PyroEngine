#pragma once

#include "Core.h"

#include <semaphore>

class Semaphore
{
public:
    explicit Semaphore(uint32 count) noexcept
        : m_semaphore(count)
    {

    }

    ~Semaphore() = default;

    Semaphore(const Semaphore& other) = delete;

    Semaphore& operator=(const Semaphore& other) = delete;

    Semaphore(Semaphore&& other) = default;

    Semaphore& operator=(Semaphore&& other) = default;
public:
    void Acquire()
    {
        m_semaphore.acquire();
    }

    void Release()
    {
        m_semaphore.release();
    }

    bool TryAcquire()
    {
        m_semaphore.try_acquire();
    }
private:
    std::counting_semaphore<> m_semaphore;
};