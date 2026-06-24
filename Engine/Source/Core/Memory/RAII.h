#pragma once

#include "Core.h"

#include <atomic>
#include <memory>

// 独占指针
template<class Type>
class ScopePtr
{
public:
    constexpr ScopePtr() = default;

    constexpr ~ScopePtr() = default;

    constexpr ScopePtr(const ScopePtr& other) = delete;

    constexpr ScopePtr& operator=(const ScopePtr& other) = delete;

    constexpr ScopePtr(ScopePtr&& other) noexcept = default;

    constexpr ScopePtr& operator=(ScopePtr&& other) noexcept = default;

    constexpr ScopePtr(std::nullptr_t) noexcept
        : m_ptr(nullptr) 
    {

    }

    constexpr ScopePtr& operator=(std::nullptr_t) noexcept
    {
        Reset(nullptr);
        return *this;
    }

    constexpr explicit ScopePtr(Type* ptr) noexcept
        : m_ptr(ptr)
    {

    }
public:
    constexpr bool IsNull() const
    {
        return m_ptr == nullptr;
    }

    constexpr void Reset(Type* ptr)
    {
        m_ptr.reset(ptr);
    }

    constexpr void Delete()
    {
        m_ptr.reset();
    }

    constexpr Type* RawPtr() const
    {
        return m_ptr.get();
    }

    constexpr void Swap(ScopePtr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }
public:
    constexpr operator bool() const
    {
        return static_cast<bool>(m_ptr);
    }

    constexpr bool operator!() const
    {
        return !m_ptr;
    }

    constexpr Type& operator*() const
    {
        return *m_ptr;
    }

    constexpr Type* operator->() const
    {
        return m_ptr.get();
    }

    constexpr friend bool operator==(const ScopePtr<Type>& left, const ScopePtr<Type>& right)
    {
        return left.m_ptr == right.m_ptr;
    }

    constexpr friend bool operator==(const Type* left, const ScopePtr<Type>& right)
    {
        return left == right.m_ptr.get();
    }

    constexpr friend bool operator==(const ScopePtr<Type>& left, const Type* right)
    {
        return left.m_ptr.get() == right;
    }

    constexpr friend bool operator!=(const ScopePtr<Type>& left, const ScopePtr<Type>& right)
    {
        return left.m_ptr != right.m_ptr;
    }

    constexpr friend bool operator!=(const Type* left, const ScopePtr<Type>& right)
    {
        return left != right.m_ptr.get();
    }
private:
    std::unique_ptr<Type> m_ptr = nullptr;
};

template<class Type, class... Args>
constexpr ScopePtr<Type> MakeScope(Args&&... args)
{
    return ScopePtr<Type>(new Type(std::forward<Args>(args)...));
}

// 共享指针
template<class Type>
class SharedPtr
{
public:
    template<class Type>
    friend class ViewPtr;
public:
    SharedPtr() = default;

    ~SharedPtr() = default;
    
    SharedPtr(const SharedPtr& other) = default;

    SharedPtr& operator=(const SharedPtr& other) = default;

    SharedPtr(SharedPtr&& other) noexcept = default;

    SharedPtr& operator=(SharedPtr&& other) noexcept = default;

    explicit SharedPtr(Type* ptr)
        : m_ptr(ptr)
    {

    }
public:
    constexpr bool IsNull() const
    {
        return !m_ptr;
    }

    constexpr void Reset(Type* ptr)
    {
        m_ptr.reset(ptr);
    }

    constexpr Type* RawPtr() const
    {
        return m_ptr.get();
    }

    constexpr void Swap(SharedPtr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }
public:
    constexpr operator bool() const
    {
        return static_cast<bool>(m_ptr);
    }

    constexpr bool operator!() const
    {
        return !m_ptr;
    }

    constexpr Type& operator*() const
    {
        return *m_ptr;
    }

    constexpr Type* operator->() const
    {
        return m_ptr.get();
    }

    constexpr friend bool operator==(const SharedPtr<Type>& left, const SharedPtr<Type>& right)
    {
        return left.m_ptr == right.m_ptr;
    }

    constexpr friend bool operator==(const Type* left, const SharedPtr<Type>& right)
    {
        return left == right.m_ptr.get();
    }

    constexpr friend bool operator==(const SharedPtr<Type>& left, const Type* right)
    {
        return left.m_ptr.get() == right;
    }

    constexpr friend bool operator!=(const SharedPtr<Type>& left, const SharedPtr<Type>& right)
    {
        return left.m_ptr != right.m_ptr;
    }

    constexpr friend bool operator!=(const Type* left, const SharedPtr<Type>& right)
    {
        return left != right.m_ptr.get();
    }
private:
    std::shared_ptr<Type> m_ptr;
};

template<class Type, class... Args>
constexpr SharedPtr<Type> MakeShared(Args&&... args)
{
    return SharedPtr<Type>(new Type(std::forward<Args>(args)...));
}

// 观察指针
template<class Type>
class ViewPtr
{
public:
    ViewPtr() = default;
    ~ViewPtr() = default;

    ViewPtr(const ViewPtr& other) noexcept = default;
    ViewPtr& operator=(const ViewPtr& other) noexcept = default;

    ViewPtr(ViewPtr&& other) noexcept = default;
    ViewPtr& operator=(ViewPtr&& other) noexcept = default;

    ViewPtr(const SharedPtr<Type>& shared) noexcept
        : m_ptr(shared.m_ptr)
    {

    }

    ViewPtr& operator=(const SharedPtr<Type>& shared) noexcept
    {
        m_ptr = shared._ptr;
        return *this;
    }

public:
    SharedPtr<Type> Lock() const
    {
        auto sp = m_ptr.lock();
        SharedPtr<Type> res;
        res._ptr = sp;
        return res;
    }

    bool IsExpired() const noexcept
    {
        return m_ptr.expired();
    }

    bool IsNull() const
    {
        return IsExpired();
    }

    Type* RawPtr() const
    {
        if (auto sp = m_ptr.lock())
            return sp.get();
        return nullptr;
    }

    void Swap(ViewPtr& other) noexcept
    {
        m_ptr.swap(other.m_ptr);
    }

public:
    explicit operator bool() const
    {
        return !IsExpired();
    }

    friend bool operator==(const ViewPtr<Type>& left, const ViewPtr<Type>& right)
    {
        return left.m_ptr.lock() == right.m_ptr.lock();
    }

    friend bool operator==(const Type* left, const ViewPtr<Type>& right)
    {
        return left == right.RawPtr();
    }

    friend bool operator==(const ViewPtr<Type>& left, const Type* right)
    {
        return left.RawPtr() == right;
    }

    friend bool operator!=(const ViewPtr<Type>& left, const ViewPtr<Type>& right)
    {
        return !(left == right);
    }

    friend bool operator!=(const Type* left, const ViewPtr<Type>& right)
    {
        return !(left == right);
    }

private:
    std::weak_ptr<Type> m_ptr;
};
// 辅助函数：从 SharedPtr 创建 ViewPtr
template<class Type>
constexpr ViewPtr<Type> MakeView(const SharedPtr<Type>& shared) noexcept
{
    return ViewPtr<Type>(shared);
}