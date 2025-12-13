#pragma once
#include "Core.h"
#include <mutex>
#include <shared_mutex>

/* 互斥锁 */
class Mutex
{
public:
    Mutex() noexcept = default;
    ~Mutex() = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
public:
    void Lock()
    {
        m_mutex.lock();
    }

    bool TryLock()
    {
        return m_mutex.try_lock();
    }

    void Unlock()
    {
        m_mutex.unlock();
    }
private:
    std::mutex m_mutex;
};

/* 读写锁 */
class RWMutex
{
public:
    RWMutex() noexcept = default;
    ~RWMutex() = default;

    RWMutex(const RWMutex&) = delete;
    RWMutex& operator=(const RWMutex&) = delete;
public:
    void LockRead()
    {
        m_mutex.lock_shared();
    }

    bool TryLockRead()
    {
        return m_mutex.try_lock_shared();
    }

    void UnlockRead()
    {
        m_mutex.unlock_shared();
    }

    void LockWrite()
    {
        m_mutex.lock();
    }

    bool TryLockWrite()
    {
        return m_mutex.try_lock();
    }

    void UnlockWrite()
    {
        m_mutex.unlock();
    }
private:
    std::shared_mutex m_mutex;
};

/* 自旋锁 */
class Spin
{
public:
    Spin() noexcept = default;
    ~Spin() = default;

    Spin(const Spin&) = delete;
    Spin& operator=(const Spin&) = delete;
public:
    void Lock()
    {
        while (flag.test_and_set());
    }

    void Unlock()
    {
        flag.clear();
    }
private:
    std::atomic_flag flag{};
};

/* 锁定策略 */
enum class LockStrategy
{
    NormalLock,     // 获得Mutex的所有权
    DeferLock,      // 不获得Mutex的所有权
    TryToLock,      // 尝试获得Mutex的所有权而不阻塞
    AdoptLock       // 领养调用方线程已拥有Mutex的所有权
};

/* 互斥锁管理工具 */
template<class MutexType>
class MutexLock
{
public:
    MutexLock() noexcept = default;

    ~MutexLock()
    {
        if (m_ownLock)
        {
            p_mutex->unlock();
        }
    }

    MutexLock(const MutexLock& other) = delete;

    MutexLock& operator=(const MutexLock& other) = delete;

    MutexLock(MutexLock&& other) noexcept
        : p_mutex(other.p_mutex)
        , m_ownLock(other.m_ownLock)
    {
        other.p_mutex = nullptr;
        other.m_ownLock = false;
    }

    MutexLock& operator=(MutexLock&& other)
    {
        if (this != std::addressof(other))
        {
            if (m_ownLock)
            {
                p_mutex->unlock();
            }

            p_mutex = other.p_mutex;
            m_ownLock = other.m_ownLock;
            other.p_mutex = nullptr;
            other.m_ownLock = false;
        }
        return *this;
    }

    explicit MutexLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        p_mutex = std::addressof(mutex);
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            p_mutex->lock();
            m_ownLock = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            m_ownLock = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            m_ownLock = p_mutex->tryLock();
            break;
        }
        case LockStrategy::AdoptLock:
        {
            m_ownLock = true;
            break;
        }
        }// switch
    }
public:
    void Lock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        p_mutex->lock();
        m_ownLock = true;
    }

    bool TryLock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownLock = p_mutex->tryLock();
        return m_ownLock;
    }

    void Unlock()
    {
        if (!p_mutex || !m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        p_mutex->unlock();
        m_ownLock = false;
    }

    void Swap(MutexLock& other)
    {
        std::swap(p_mutex, other.p_mutex);
        std::swap(m_ownLock, other.m_ownLock);
    }

    MutexType* Release()
    {
        MutexType* res = p_mutex;
        p_mutex = nullptr;
        m_ownLock = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return p_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownLock;
    }
public:

private:
    MutexType* p_mutex = nullptr;
    bool m_ownLock = false;
};

/* 读锁管理工具 */
template<class MutexType>
class ReadLock
{
public:
    ReadLock() noexcept = default;

    ~ReadLock()
    {
        if (m_ownLock)
        {
            p_mutex->unlockRead();
        }
    }

    ReadLock(const ReadLock& other) = delete;

    ReadLock& operator=(const ReadLock& other) = delete;

    ReadLock(ReadLock&& other) noexcept
        : p_mutex(other.p_mutex)
        , m_ownLock(other.m_ownLock)
    {
        other.p_mutex = nullptr;
        other.m_ownLock = false;
    }

    ReadLock& operator=(ReadLock&& other)
    {
        if (this != std::addressof(other))
        {
            if (m_ownLock)
            {
                p_mutex->unlockRead();
            }

            p_mutex = other._Pmtx;
            m_ownLock = other.m_ownLock;
            other.p_mutex = nullptr;
            other.m_ownLock = false;
        }
        return *this;
    }

    explicit ReadLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            p_mutex = std::addressof(mutex);
            p_mutex->lockRead();
            m_ownLock = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock(p_mutex->tryLockRead());
            break;
        }
        case LockStrategy::AdoptLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock = true;
            break;
        }
        }
    }
public:
    void Lock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        p_mutex->lockRead();
        m_ownLock = true;
    }

    bool TryLock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownLock = p_mutex->tryLockRead();
        return m_ownLock;
    }

    void Unlock()
    {
        if (!p_mutex || !m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        p_mutex->unlockRead();
        m_ownLock = false;
    }

    MutexType* Release()
    {
        MutexType* res = p_mutex;
        p_mutex = nullptr;
        m_ownLock = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return p_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownLock;
    }

    void Swap(ReadLock& other)
    {
        std::swap(p_mutex, other.p_mutex);
        std::swap(m_ownLock, other.m_ownLock);
    }
private:
    MutexType* p_mutex = nullptr;
    bool m_ownLock = false;
};

/* 写锁管理工具 */
template<class MutexType>
class WriteLock
{
public:
    WriteLock() noexcept = default;

    ~WriteLock()
    {
        if (m_ownLock)
        {
            p_mutex->unlockWrite();
        }
    }

    WriteLock(const WriteLock& other) = delete;

    WriteLock& operator=(const WriteLock& other) = delete;

    WriteLock(WriteLock&& other) noexcept
        : p_mutex(other.p_mutex)
        , m_ownLock(other.m_ownLock)
    {
        other.p_mutex = nullptr;
        other.m_ownLock = false;
    }

    WriteLock& operator=(WriteLock&& other)
    {
        if (this != std::addressof(other))
        {
            if (m_ownLock)
            {
                p_mutex->unlockWrite();
            }

            p_mutex = other._Pmtx;
            m_ownLock = other.m_ownLock;
            other.p_mutex = nullptr;
            other.m_ownLock = false;
        }
        return *this;
    }

    explicit WriteLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            p_mutex = std::addressof(mutex);
            p_mutex->lockWrite();
            m_ownLock = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock(p_mutex->tryLockWrite());
            break;
        }
        case LockStrategy::AdoptLock:
        {
            p_mutex = std::addressof(mutex);
            m_ownLock = true;
            break;
        }
        }
    } 
public:
    void Lock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        p_mutex->lockWrite();
        m_ownLock = true;
    }

    bool TryLock()
    {
        if (!p_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownLock = p_mutex->tryLockWrite();
        return m_ownLock;
    }

    void Unlock()
    {
        if (!p_mutex || !m_ownLock)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        p_mutex->unlockWrite();
        m_ownLock = false;
    }

    MutexType* Release()
    {
        MutexType* res = p_mutex;
        p_mutex = nullptr;
        m_ownLock = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return p_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownLock;
    }

    void Swap(WriteLock& other)
    {
        std::swap(p_mutex, other.p_mutex);
        std::swap(m_ownLock, other.m_ownLock);
    }
private:
    MutexType* p_mutex = nullptr;
    bool m_ownLock = false;
};