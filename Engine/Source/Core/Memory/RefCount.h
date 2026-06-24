#pragma once

#include "Core.h"
#include "Concept/Concept.h"

#include <atomic>

/// <summary>
/// 线程安全引用计数基类
/// </summary>
class RefCount
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    RefCount() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~RefCount() = default;
    /// <summary>
    /// 禁用拷贝构造函数
    /// 引用计数对象不允许拷贝，否则会导致计数混乱
    /// </summary>
    RefCount(const RefCount& other) = delete;
    /// <summary>
    /// 禁用拷贝赋值函数
    /// 引用计数对象不允许拷贝，否则会导致计数混乱
    /// </summary>
    RefCount& operator=(const RefCount& other) = delete;
    /// <summary>
    /// 禁用移动构造函数
    /// 引用计数对象不允许移动，保证对象地址唯一
    /// </summary>
    RefCount(RefCount&& other) noexcept = delete;
    /// <summary>
    /// 禁用移动赋值函数
    /// 引用计数对象不允许移动，保证对象地址唯一
    /// </summary>
    RefCount& operator=(RefCount&& other) noexcept = delete;
public:
    /// <summary>
    /// 增加引用计数
    /// <para>内存序：acq_rel</para>
    /// </summary>
    void AddRef()
    {
        m_refCount.fetch_add(1, std::memory_order_acq_rel);
    }
    /// <summary>
    /// 减少引用计数
    /// <para>内存序：acq_rel</para>
    /// </summary>
    void Release()
    {
        if (m_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            DeleteObject();
        }
    }
    /// <summary>
    /// 获取当前引用计数
    /// <para>内存序：relaxed</para>
    /// </summary>
    /// <returns>引用计数</returns>
    int32 GetRefCount() const
    {
        return m_refCount.load(std::memory_order_relaxed);
    }
public:
    /// <summary>
    /// 
    /// </summary>
    virtual void DeleteObject()
    {
        delete this;
    }
private:
    /// <summary>
    /// 原子引用计数
    /// </summary>
    mutable std::atomic<int32> m_refCount = 1;
};

namespace Concept
{
/// <summary>
/// 
/// </summary>
template<class Type>
concept RefCountType = std::derived_from<Type, RefCount> || std::same_as<Type, RefCount>;
} // Concept

/// <summary>
/// 线程安全引用计数智能指针
/// </summary>
/// <typeparam name="Type"></typeparam>
template<Concept::RefCountType Type>
class RefPtr
{
public:
    // 让所有类型的 RefPtr 互为友元，用于派生类→基类转换
    template<Concept::RefCountType U>
    friend class RefPtr;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr RefPtr() noexcept = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~RefPtr()
    {
        if (m_ptr)
        {
            m_ptr->Release();
        }
    }
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other"></param>
    RefPtr(const RefPtr& other) noexcept
        : m_ptr(other.m_ptr)
    {
        if (m_ptr)
        {
            m_ptr->AddRef();
        }
    }
    /// <summary>
    /// 拷贝赋值函数
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    RefPtr& operator=(const RefPtr& other) noexcept
    {
        if (this != &other)
        {
            Reset(other.m_ptr);
        }
        return *this;
    }
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other"></param>
    RefPtr(RefPtr&& other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    RefPtr& operator=(RefPtr&& other) noexcept
    {
        if (this != &other)
        {
            // 保存旧指针
            Type* oldPtr = m_ptr;

            // 接管新指针
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;

            // 释放旧指针
            if (oldPtr)
            {
                oldPtr->Release();
            }
        }
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    constexpr RefPtr(std::nullptr_t) noexcept
    {

    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ptr"></param>
    explicit RefPtr(Type* ptr) noexcept
        : m_ptr(ptr)
    {

    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ptr"></param>
    /// <returns></returns>
    RefPtr& operator=(Type* ptr) noexcept
    {
        if (ptr != m_ptr)  // 检查是否自赋值
        {
            Reset(ptr);
        }
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="U"></typeparam>
    /// <param name="other"></param>
    template<Concept::DerivedFrom<Type> U>
    RefPtr(const RefPtr<U>& other) noexcept
        : m_ptr(other.RawPtr())
    {
        if (m_ptr)
        {
            m_ptr->AddRef();
        }
    }
    // ✅ 关键修复：支持子类指针赋值给父类智能指针
    template<Concept::DerivedFrom<Type> U>
    RefPtr& operator=(const RefPtr<U>& other) noexcept 
    { 
        Reset(other.RawPtr()); return *this; 
    }
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="U"></typeparam>
    /// <param name="other"></param>
    template<Concept::DerivedFrom<Type> U>
    RefPtr(RefPtr<U>&& other) noexcept
        : m_ptr(other.RawPtr())
    {
        other.Reset();
    }
public:
    /// <summary>
    /// 重置智能指针
    /// </summary>
    /// <param name="ptr"></param>
    void Reset(Type* ptr = nullptr) noexcept
    {
        if (m_ptr == ptr)
        {
            return;
        }

        // 先加新引用，再减旧引用，防止自赋值/中途释放
        if (ptr)
        {
            ptr->AddRef();
        }

        // 释放原有指针
        if (m_ptr)
        {
            m_ptr->Release();
        }

        m_ptr = ptr;
    }

    void Destroy() noexcept
    {
        if (m_ptr)
        {
            Type* toDelete = m_ptr;   // 保存原始指针
            m_ptr = nullptr;          // 先置空，防止重入或并发访问

            // 强制删除对象（无论当前引用计数是多少）
            toDelete->DeleteObject(); // DeleteObject() 内部会执行 delete this
        }
    }
    /// <summary>
    /// 获取原始裸指针
    /// </summary>
    /// <returns></returns>
    Type* RawPtr() const noexcept
    {
        return m_ptr;
    } 
    /// <summary>
    /// 获取对象当前引用计数，空指针返回 0
    /// </summary>
    /// <returns></returns>
    int32 GetRefCount() const noexcept
    {
        return m_ptr ? m_ptr->GetRefCount() : 0;
    }
    /// <summary>
    /// 交换对象
    /// </summary>
    /// <param name="other"></param>
    void Swap(RefPtr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }
public:
    /// <summary>
    /// 
    /// </summary>
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    Type* operator->() const noexcept
    {
        assert(m_ptr != nullptr);
        return m_ptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    Type& operator*() const noexcept
    {
        assert(m_ptr != nullptr);
        return *m_ptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    bool operator==(const RefPtr& other) const noexcept
    {
        return m_ptr == other.m_ptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    bool operator!=(const RefPtr& other) const noexcept
    {
        return m_ptr != other.m_ptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    bool operator<(const RefPtr& other) const noexcept
    {
        return m_ptr < other.m_ptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool operator==(std::nullptr_t) const noexcept
    {
        return m_ptr == nullptr;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool operator!=(std::nullptr_t) const noexcept
    {
        return m_ptr != nullptr;
    }
private:
    /// <summary>
    /// 
    /// </summary>
    Type* m_ptr = nullptr;
};

/// <summary>
/// 
/// </summary>
/// <typeparam name="...Args"></typeparam>
/// <typeparam name="Type"></typeparam>
/// <param name="...args"></param>
/// <returns></returns>
template<Concept::RefCountType Type, class... Args>
[[nodiscard]] RefPtr<Type> MakeRef(Args&&... args)
{
    return RefPtr<Type>(new Type(std::forward<Args>(args)...));
}