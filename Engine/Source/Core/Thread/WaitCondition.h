#pragma once

#include "Core.h"

#include <condition_variable>

class WaitCondition
{
public:
    WaitCondition() = default;

    ~WaitCondition() = default;

    WaitCondition(const WaitCondition&) = delete;

    WaitCondition& operator=(const WaitCondition&) = delete;

    WaitCondition(WaitCondition&& other) = default;

    WaitCondition& operator=(WaitCondition&& other) = default;
public:
    template<class Lock>
    void Wait(Lock& lock)
    {
        m_any_cv.wait(lock);
    }

    template<class Lock, class Predicate>
    void Wait(Lock& lock, Predicate pred)
    {
        m_any_cv.wait(lock, pred);
    }

    void NotifyOne()
    {
        m_any_cv.notify_one();
    }

    void NotifyAll()
    {
        m_any_cv.notify_all();
    }
private:
    std::condition_variable_any m_any_cv;
};