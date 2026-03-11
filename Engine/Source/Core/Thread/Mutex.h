#pragma once

#include "Core.h"

#include <mutex>
#include <shared_mutex>

/// <summary>
/// 自旋锁
/// </summary>
class Spin
{
public:
    Spin() noexcept = default;
    ~Spin() = default;

    Spin(const Spin&) = delete;
    Spin& operator=(const Spin&) = delete;

    void Lock()
    {
        while (m_flag.test_and_set());
    }

    void Unlock()
    {
        m_flag.clear();
    }
private:
    std::atomic_flag m_flag;
};

/// <summary>
/// 互斥锁
/// </summary>
class Mutex
{
public:
    Mutex() noexcept = default;
    ~Mutex() = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

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

/// <summary>
/// 读写锁
/// </summary>
class RWMutex
{
public:
    RWMutex() noexcept = default;
    ~RWMutex() = default;

    RWMutex(const RWMutex&) = delete;
    RWMutex& operator=(const RWMutex&) = delete;

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

/// <summary>
/// 锁定策略
/// </summary>
enum class LockStrategy
{
    NormalLock,     // 获得Mutex的所有权
    DeferLock,      // 不获得Mutex的所有权
    TryToLock,      // 尝试获得Mutex的所有权而不阻塞
    AdoptLock       // 领养调用方线程已拥有Mutex的所有权

};

/// <summary>
/// 互斥锁管理工具
/// </summary>
/// <typeparam name="MutexType"></typeparam>
template<class MutexType>
class MutexLock
{
public:
    MutexLock() noexcept = default;

    ~MutexLock()
    {
        if (m_ownFlag)
        {
            m_mutex->Unlock();
        }
    }

    MutexLock(const MutexLock& other) = delete;
    MutexLock& operator=(const MutexLock& other) = delete;

    MutexLock(MutexLock&& other) noexcept
        : m_mutex(other.m_mutex)
        , m_ownFlag(other.m_ownFlag)
    {
        other.m_mutex = nullptr;
        other.m_ownFlag = false;
    }

    MutexLock& operator=(MutexLock&& other) noexcept
    {
        if (this != std::addressof(other))
        {
            if (m_ownFlag)
            {
                m_mutex->Unlock();
            }

            m_mutex = other.m_mutex;
            m_ownFlag = other.m_ownFlag;
            other.m_mutex = nullptr;
            other.m_ownFlag = false;
        }
        return *this;
    }

    explicit MutexLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        m_mutex = std::addressof(mutex);
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            m_mutex->Lock();
            m_ownFlag = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            m_ownFlag = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            m_ownFlag = m_mutex->TryLock();
            break;
        }
        case LockStrategy::AdoptLock:
        {
            m_ownFlag = true;
            break;
        }
        }// switch
    }
public:
    void Swap(MutexLock& other) noexcept
    {
        std::swap(m_mutex, other.m_mutex);
        std::swap(m_ownFlag, other.m_ownFlag);
    }

    void Lock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_mutex->lock();
        m_ownFlag = true;
    }

    bool TryLock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownFlag = m_mutex->tryLock();
        return m_ownFlag;
    }

    void Unlock()
    {
        if (!m_mutex || !m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        m_mutex->Unlock();
        m_ownFlag = false;
    }
public:
    MutexType* Release()
    {
        MutexType* res = m_mutex;
        m_mutex = nullptr;
        m_ownFlag = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return m_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownFlag;
    }
private:
    MutexType* m_mutex = nullptr;
    bool m_ownFlag = false;
};

/// <summary>
/// 读锁管理工具
/// </summary>
/// <typeparam name="MutexType"></typeparam>
template<class MutexType>
class ReadLock
{
public:
    ReadLock() noexcept = default;

    ~ReadLock()
    {
        if (m_ownFlag)
        {
            m_mutex->UnlockRead();
        }
    }



    ReadLock(const ReadLock& other) = delete;
    ReadLock& operator=(const ReadLock& other) = delete;

    ReadLock(ReadLock&& other) noexcept
        : m_mutex(other.m_mutex)
        , m_ownFlag(other.m_ownFlag)
    {
        other.m_mutex = nullptr;
        other.m_ownFlag = false;
    }

    ReadLock& operator=(ReadLock&& other)
    {
        if (this != std::addressof(other))
        {
            if (m_ownFlag)
            {
                m_mutex->unlockRead();
            }

            m_mutex = other.m_mutex;
            m_ownFlag = other.m_ownFlag;
            other.m_mutex = nullptr;
            other.m_ownFlag = false;
        }
        return *this;
    }

    explicit ReadLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            m_mutex = std::addressof(mutex);
            m_mutex->LockRead();
            m_ownFlag = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag(m_mutex->TryLockRead());
            break;
        }
        case LockStrategy::AdoptLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag = true;
            break;
        }
        }
    }
public:
    void Swap(ReadLock& other)
    {
        std::swap(m_mutex, other.m_mutex);
        std::swap(m_ownFlag, other.m_ownFlag);
    }

    void Lock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_mutex->LockRead();
        m_ownFlag = true;
    }

    bool TryLock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownFlag = m_mutex->TryLockRead();
        return m_ownFlag;
    }

    void Unlock()
    {
        if (!m_mutex || !m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        m_mutex->UnlockRead();
        m_ownFlag = false;
    }
public:
    MutexType* Release()
    {
        MutexType* res = m_mutex;
        m_mutex = nullptr;
        m_ownFlag = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return m_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownFlag;
    }
private:
    MutexType* m_mutex = nullptr;
    bool m_ownFlag = false;
};

/// <summary>
/// 写锁管理工具
/// </summary>
/// <typeparam name="MutexType"></typeparam>
template<class MutexType>
class WriteLock
{
public:
    WriteLock() noexcept = default;

    ~WriteLock()
    {
        if (m_ownFlag)
        {
            m_mutex->UnlockWrite();
        }
    }

    explicit WriteLock(MutexType& mutex, LockStrategy strategy = LockStrategy::NormalLock)
    {
        switch (strategy)
        {
        case LockStrategy::NormalLock:
        {
            m_mutex = std::addressof(mutex);
            m_mutex->LockWrite();
            m_ownFlag = true;
            break;
        }
        case LockStrategy::DeferLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag = false;
            break;
        }
        case LockStrategy::TryToLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag(m_mutex->TryLockWrite());
            break;
        }
        case LockStrategy::AdoptLock:
        {
            m_mutex = std::addressof(mutex);
            m_ownFlag = true;
            break;
        }
        }
    }

    WriteLock(const WriteLock& other) = delete;
    WriteLock& operator=(const WriteLock& other) = delete;

    WriteLock(WriteLock&& other) noexcept
        : m_mutex(other.m_mutex)
        , m_ownFlag(other.m_ownFlag)
    {
        other.m_mutex = nullptr;
        other.m_ownFlag = false;
    }

    WriteLock& operator=(WriteLock&& other)
    {
        if (this != std::addressof(other))
        {
            if (m_ownFlag)
            {
                m_mutex->UnlockWrite();
            }

            m_mutex = other._Pmtx;
            m_ownFlag = other.m_ownFlag;
            other.m_mutex = nullptr;
            other.m_ownFlag = false;
        }
        return *this;
    }

    void Swap(WriteLock& other)
    {
        std::swap(m_mutex, other.m_mutex);
        std::swap(m_ownFlag, other.m_ownFlag);
    }
public:
    void Lock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_mutex->LockWrite();
        m_ownFlag = true;
    }

    bool TryLock()
    {
        if (!m_mutex)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        if (m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::resource_deadlock_would_occur), "Resource deadlock would occur");
        }
        m_ownFlag = m_mutex->TryLockWrite();
        return m_ownFlag;
    }

    void Unlock()
    {
        if (!m_mutex || !m_ownFlag)
        {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted), "Operation not permitted");
        }

        m_mutex->UnlockWrite();
        m_ownFlag = false;
    }
public:
    MutexType* Release()
    {
        MutexType* res = m_mutex;
        m_mutex = nullptr;
        m_ownFlag = false;
        return res;
    }

    MutexType* GetMutex()
    {
        return m_mutex;
    }

    bool IsOwnLock()
    {
        return m_ownFlag;
    }
private:
    MutexType* m_mutex = nullptr;
    bool m_ownFlag = false;
};