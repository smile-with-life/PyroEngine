#pragma once
#include "Core.h"
#include "Concept/Concept.h"

/// <summary>
/// 独占指针
/// </summary>
/// <typeparam name="Type">类类型</typeparam>
template<Concept::ClassType Type>
class ScopePtr
{
public:
    constexpr ScopePtr() = default;

    constexpr ~ScopePtr()
    {
        if (m_ptr)
        {
            delete m_ptr;
        }
    }

    constexpr ScopePtr(const ScopePtr& other) = delete;

    constexpr ScopePtr& operator=(const ScopePtr& other) = delete;

    constexpr ScopePtr(ScopePtr&& other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    constexpr ScopePtr& operator=(ScopePtr&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
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
        if (ptr == m_ptr) [[unlikely]]
        {
            return;
        }
        delete m_ptr;
        m_ptr = ptr;
    }

    constexpr void Delete()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    constexpr Type* RawPtr() const
    {
        return m_ptr;
    }

    constexpr void Swap(ScopePtr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }
public:
    constexpr ScopePtr& operator=(std::nullptr_t) noexcept
    {
        Reset();
        return *this;
    }

    constexpr operator bool() const
    {
        return static_cast<bool>(m_ptr);
    }

    constexpr bool operator!() const
    {
        return !static_cast<bool>(m_ptr);
    }

    constexpr Type& operator*() const
    {
        return *m_ptr;
    }

    constexpr Type* operator->() const
    {
        return m_ptr;
    }

    constexpr friend bool operator==(const ScopePtr<Type>& left, const ScopePtr<Type>& right)
    {
        return left.m_ptr == right.m_ptr;
    }

    //template<class Type>
    constexpr friend bool operator==(const Type* left, const ScopePtr<Type>& right)
    {
        return left == right.m_ptr;
    }

    //template<class Type>
    constexpr friend bool operator==(const ScopePtr<Type>& left, const Type* right)
    {
        return left.m_ptr == right;
    }

    //template<class Type>
    constexpr friend bool operator!=(const ScopePtr<Type>& left, const ScopePtr<Type>& right)
    {
        return left.m_ptr != right.m_ptr;
    }

    //template<class Type>
    constexpr friend bool operator!=(const Type* left, const ScopePtr<Type>& right)
    {
        return left != right.m_ptr;
    }
private:
    Type* m_ptr = nullptr;
};

template<Concept::ClassType Type, class... Args>
constexpr ScopePtr<Type> MakeScope(Args&&... args)
{
    return ScopePtr<Type>(new Type(std::forward<Args>(args)...));
}

/// <summary>
/// 视图指针
/// </summary>
/// <typeparam name="Type"></typeparam>
template<Concept::ClassType Type>
class ViewPtr
{
public:
    constexpr ViewPtr() = default;

    constexpr ~ViewPtr() = default;

    constexpr ViewPtr(const ViewPtr& other)
        : m_ptr(other.m_ptr)
    {
        
    }

    constexpr ViewPtr& operator=(const ViewPtr& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }

    constexpr ViewPtr(ViewPtr&& other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    constexpr ViewPtr& operator=(ViewPtr&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    constexpr explicit ViewPtr(Type* ptr) noexcept
        : m_ptr(ptr)
    {

    }
public:
    constexpr Type* RawPtr() const
    {
        return m_ptr;
    }

    constexpr bool IsValid() const
    {
        return m_ptr != nullptr;
    }

    constexpr bool IsNull() const
    {
        return m_ptr == nullptr;
    }

    constexpr void Swap(ViewPtr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }
public:
    constexpr ViewPtr& operator=(std::nullptr_t) noexcept
    {
        m_ptr = nullptr;
        return *this;
    }

    constexpr operator bool() const
    {
        return static_cast<bool>(m_ptr);
    }

    constexpr bool operator!() const
    {
        return !static_cast<bool>(m_ptr);
    }

    constexpr Type* operator->() const
    {
        return m_ptr;
    }

    constexpr friend bool operator==(const ViewPtr& left, const ViewPtr& right)
    {
        return left.m_ptr == right.m_ptr;
    }

    constexpr friend bool operator==(const Type* left, const ViewPtr& right)
    {
        return left == right.m_ptr;
    }

    constexpr friend bool operator==(const ViewPtr& left, const Type* right)
    {
        return left.m_ptr == right;
    }

    constexpr friend bool operator!=(const ViewPtr& left, const ViewPtr& right)
    {
        return left.m_ptr != right.m_ptr;
    }

    constexpr friend bool operator!=(const Type* left, const ViewPtr& right)
    {
        return left != right.m_ptr;
    }

    constexpr friend bool operator!=(const ViewPtr& left, const Type* right)
    {
        return left.m_ptr != right;
    }
private:
    Type* m_ptr = nullptr;
};