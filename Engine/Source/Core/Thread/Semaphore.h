#pragma once
#include "Core.h"
#include "Time/Time.h"
#include "Thread/Mutex.h"
#include "Thread/WaitCondition.h"

class Semaphore
{
public:
    explicit Semaphore(uint64 num) noexcept;

    ~Semaphore() = default;

    Semaphore(const Semaphore&) = delete;

    Semaphore& operator=(const Semaphore&) = delete;
public:
    void Acquire(uint64 num = 1);

    void Release(uint64 num = 1);

    bool TryAcquire(uint64 num = 1);

    bool TryAcquire(uint64 num, Msecsecond msecs);

    uint64 Count();
private:
    Mutex m_mutex;
    WaitCondition m_cv;
    uint64 m_count;
};