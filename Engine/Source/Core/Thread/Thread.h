#pragma once
#include "Core.h"
#include <thread>

// 线程id
class ThreadId
{
public:
public:
    friend class Thread;
public:
    ThreadId() noexcept = default;

    ThreadId(const ThreadId& other) = default;
    ThreadId& operator=(const ThreadId& other) = default;

    ThreadId(ThreadId&& other) noexcept = default;
    ThreadId& operator=(ThreadId&& other) noexcept = default;
public:
    friend bool operator==(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id == right.m_id;
    }

    friend bool operator!=(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id != right.m_id;
    }

    friend bool operator>(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id > right.m_id;
    }

    friend bool operator>=(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id >= right.m_id;
    }

    friend bool operator<(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id < right.m_id;
    }

    friend bool operator<=(ThreadId left, ThreadId right) noexcept
    {
        return left.m_id <= right.m_id;
    }
private:
    explicit ThreadId(std::thread::id id) noexcept
        : m_id(*(uint*)&id)
    {

    }
private:
    uint32 m_id = 0;
};

// 线程
class Thread
{

public:
    Thread() = default;

    ~Thread()
    {
        if (Joinable())
        {
            Join();
        }
    }

    template<class Func, class... Args>
    Thread(Func&& func, Args&&... args)
    {
        m_thread = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    Thread(const Thread&) = delete;
    Thread& operator=(Thread& other) = delete;

    Thread(Thread&& other) noexcept = default;
    Thread& operator=(Thread&& other) noexcept = default;
public:
    void Join()
    {
        m_thread.join();
    }

    bool Joinable()
    {
        return m_thread.joinable();
    }

    void Detach()
    {
        m_thread.detach();
    }

    ThreadId GetId() const noexcept
    {
        return ThreadId(m_thread.get_id());
    }

    void* NativeHandle()
    {
        return m_thread.native_handle();
    }

    void Swap(Thread& other) noexcept
    {
        std::swap(m_thread, other.m_thread);
    }
public:
    // 强制当前线程休眠 msecs 毫秒
    static void Sleep(uint64 msecs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msecs));
    }

    static unsigned int HardwareConcurrency() noexcept
    {
        return std::thread::hardware_concurrency();
    }

    static ThreadId CurrentThreadId() noexcept
    {
        return ThreadId(std::this_thread::get_id());
    }
private:
    std::thread m_thread;
};