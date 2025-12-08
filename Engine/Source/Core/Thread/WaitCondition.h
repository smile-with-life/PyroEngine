#pragma once
#include "Core.h"
#include "Time/Time.h"
#include <condition_variable>

class WaitCondition
{
public:
    WaitCondition() = default;

    ~WaitCondition() = default;

    WaitCondition(const WaitCondition&) = delete;

    WaitCondition& operator=(const WaitCondition&) = delete;
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

    template<class Lock>
    bool WaitFor(Lock& lock, Milliseconds msecs)
    {
        return m_any_cv.Wait_for(lock, std::chrono::milliseconds(msecs));
    }

    template<class Lock, class Predicate>
    bool WaitFor(Lock& lock, Milliseconds msecs, Predicate pred)
    {
        return m_any_cv.wait_for(lock, std::chrono::milliseconds(msecs), pred);
    }

    void NotifyOne();

    void NotifyAll();
private:
    std::condition_variable_any m_any_cv;
};