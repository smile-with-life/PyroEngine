#pragma once

#include "Core.h"
#include "Allocator/Allocator.h"
#include "Iterator/Iterator.h"
#include "Diagnosis/Debug.h"

#include <utility>
#include <type_traits>
#include <limits>

template <class Type>
class ArrayIterator
{
public:
    using iterator_concept = ContiguousIteratorTag;
    using iterator_category = RandomAccessIteratorTag;
    using difference_type = ptrdiff;
    using value_type = Type;
    using reference = Type&;
public:
    constexpr ArrayIterator() noexcept
        : m_ptr()
    {

    }

    explicit constexpr ArrayIterator(Type* ptr) noexcept
        : m_ptr(ptr)
    {

    }
public:
    [[nodiscard]] constexpr Type& operator[](const ptrdiff off) const noexcept
    {
        return *(*this + off);
    }

    [[nodiscard]] constexpr Type* operator->() const noexcept
    {
        return m_ptr;
    }

    [[nodiscard]] constexpr Type& operator*() const noexcept
    {
        return *m_ptr;
    }

    constexpr ArrayIterator& operator++() noexcept
    {
        ++m_ptr;
        return *this;
    }

    constexpr ArrayIterator operator++(int) noexcept
    {
        ArrayIterator tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr ArrayIterator& operator--() noexcept
    {
        --m_ptr;
        return *this;
    }

    constexpr ArrayIterator operator--(int) noexcept
    {
        ArrayIterator tmp = *this;
        --*this;
        return tmp;
    }

    [[nodiscard]] constexpr ArrayIterator operator+(const ptrdiff off) const noexcept
    {
        ArrayIterator tmp = *this;
        tmp += off;
        return tmp;
    }

    [[nodiscard]] friend constexpr ArrayIterator operator+(const ptrdiff off, ArrayIterator next) noexcept
    {
        next += off;
        return next;
    }

    constexpr ArrayIterator& operator+=(const ptrdiff off) noexcept
    {
        m_ptr += off;
        return *this;
    }

    [[nodiscard]] constexpr ArrayIterator operator-(const ptrdiff off) const noexcept
    {
        ArrayIterator tmp = *this;
        tmp -= off;
        return tmp;
    }

    [[nodiscard]] constexpr ptrdiff operator-(const ArrayIterator& right) const noexcept
    {
        return static_cast<ptrdiff>(m_ptr - right.m_ptr);
    }

    constexpr ArrayIterator& operator-=(const ptrdiff off) noexcept
    {
        return *this += -off;
    }

    [[nodiscard]] constexpr bool operator==(const ArrayIterator& right) const noexcept
    {
        return m_ptr == right.m_ptr;
    }

    [[nodiscard]] constexpr bool operator!=(const ArrayIterator& right) const noexcept
    {
        return !(*this == right);
    }

    [[nodiscard]] constexpr bool operator<(const ArrayIterator& right) const noexcept
    {
        return m_ptr < right.m_ptr;
    }

    [[nodiscard]] constexpr bool operator<=(const ArrayIterator& right) const noexcept
    {
        return !(right < *this);
    }

    [[nodiscard]] constexpr bool operator>(const ArrayIterator& right) const noexcept
    {
        return right < *this;
    }

    [[nodiscard]] constexpr bool operator>=(const ArrayIterator& right) const noexcept
    {
        return !(*this < right);
    }
private:
    Type* m_ptr = nullptr;
};

template <class Type>
class ArrayConstIterator
{
public:
    using iterator_concept = ContiguousIteratorTag;
    using iterator_category = RandomAccessIteratorTag;
    using difference_type = ptrdiff;
    using value_type = Type;
    using reference = const Type&;
public:
    constexpr ArrayConstIterator() noexcept
        : m_ptr()
    {

    }

    explicit constexpr ArrayConstIterator(const Type* ptr) noexcept
        : m_ptr(ptr)
    {

    }
public:
    [[nodiscard]] constexpr const Type& operator[](const ptrdiff off) const noexcept
    {
        return *(*this + off);
    }

    [[nodiscard]] constexpr const Type* operator->() const noexcept
    {
        return m_ptr;
    }

    [[nodiscard]] constexpr const Type& operator*() const noexcept
    {
        return *m_ptr;
    }

    constexpr ArrayConstIterator& operator++() noexcept
    {
        ++m_ptr;
        return *this;
    }

    constexpr ArrayConstIterator operator++(int) noexcept
    {
        ArrayConstIterator tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr ArrayConstIterator& operator--() noexcept
    {
        --m_ptr;
        return *this;
    }

    constexpr ArrayConstIterator operator--(int) noexcept
    {
        ArrayConstIterator tmp = *this;
        --*this;
        return tmp;
    }

    [[nodiscard]] constexpr ArrayConstIterator operator+(const ptrdiff off) const noexcept
    {
        ArrayConstIterator tmp = *this;
        tmp += off;
        return tmp;
    }

    [[nodiscard]] friend constexpr ArrayConstIterator operator+(const ptrdiff off, ArrayConstIterator next) noexcept
    {
        next += off;
        return next;
    }

    constexpr ArrayConstIterator& operator+=(const ptrdiff off) noexcept
    {
        m_ptr += off;
        return *this;
    }

    [[nodiscard]] constexpr ArrayConstIterator operator-(const ptrdiff off) const noexcept
    {
        ArrayConstIterator tmp = *this;
        tmp -= off;
        return tmp;
    }

    [[nodiscard]] constexpr ptrdiff operator-(const ArrayConstIterator& right) const noexcept
    {
        return static_cast<ptrdiff>(m_ptr - right.m_ptr);
    }

    constexpr ArrayConstIterator& operator-=(const ptrdiff off) noexcept
    {
        return *this += -off;
    }

    [[nodiscard]] constexpr bool operator==(const ArrayConstIterator& right) const noexcept
    {
        return m_ptr == right.m_ptr;
    }

    [[nodiscard]] constexpr bool operator!=(const ArrayConstIterator& right) const noexcept
    {
        return !(*this == right);
    }

    [[nodiscard]] constexpr bool operator<(const ArrayConstIterator& right) const noexcept
    {
        return m_ptr < right.m_ptr;
    }

    [[nodiscard]] constexpr bool operator<=(const ArrayConstIterator& right) const noexcept
    {
        return !(right < *this);
    }

    [[nodiscard]] constexpr bool operator>(const ArrayConstIterator& right) const noexcept
    {
        return right < *this;
    }

    [[nodiscard]] constexpr bool operator>=(const ArrayConstIterator& right) const noexcept
    {
        return !(*this < right);
    }
private:
    const Type* m_ptr = nullptr;
};

template<class Type>
class Array
{
public:
    using value_type = Type;
    using size_type = int64;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ArrayIterator<value_type>;
    using const_iterator = ArrayConstIterator<value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr Array() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~Array()
    {
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        if (m_capacity != 0)
        {
            m_alloc.Deallocate(m_data, m_capacity);
        }
    }
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    constexpr Array(const Array& other)
        : m_alloc(other.m_alloc)
    {
        m_capacity = m_size = other.m_size;
        if (m_size != 0)
        {
            m_data = m_alloc.Allocate(m_capacity);
            for (size_type i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
            }
        }
        else
        {
            m_data = nullptr;
        }
    }
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    constexpr Array& operator=(const Array& other)
    {
        // 判断自赋值
        if (&other == this) [[unlikely]]
        {
            return *this;
        }
        Clear();
        Reserve(other.m_size);
        m_size = other.m_size;
        for (size_type i = 0; i != m_size; i++)
        {
            std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
        }

        return *this;
    }
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    constexpr Array(Array&& other)
        : m_alloc(std::move(other.m_alloc))
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    constexpr Array& operator=(Array&& other)
    {
        // 判断自赋值
        if (&other == this) [[unlikely]]
        {
            return *this;
        }
        Clear();
        if (m_capacity != 0)
        {
            m_alloc.Deallocate(m_data, m_capacity);
        }
        // 接管 other 的资源
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_alloc = other.m_alloc;
        // 将 other 置为空容器 
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }
    /// <summary>
    /// 构造函数，创建指定大小的容器
    /// </summary>
    /// <param name="count">容器初始大小</param>
    constexpr explicit Array(size_type count)
    {
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i]);
        }
    }
    /// <summary>
    /// 构造函数，指定容器大小并赋初值
    /// </summary>
    /// <param name="count">容器初始大小</param>
    /// <param name="value">用于填充的初始值</param>
    constexpr Array(size_type count, const Type& value)
    {
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], value);
        }
    }
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">输入迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template <class InputIt>
    constexpr Array(InputIt first, InputIt last)
    {
        size_type count = last - first;
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], *first);
            first++;
        }
    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr Array(std::initializer_list<Type> ilist)
        : Array(ilist.begin(), ilist.end())
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& operator=(std::initializer_list<Type> ilist)
    {
        auto first = ilist.begin();
        auto last = ilist.end();
        size_type count = last - first;
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], *first);
            first++;
        }
        return *this;
    }
public:
    /// <summary>
    /// 访问指定位置的元素
    /// </summary>
    /// <param name="index">位置索引</param>
    /// <returns>返回元素的引用</returns>
    /// <exception cref="std::out_of_range">当index超出范围时抛出</exception>
    constexpr Type& At(size_type index)
    {
        checkf(IsValidIndex(index));

        return m_data[index];
    }
    /// <summary>
    /// 访问指定位置的元素(const版本)
    /// </summary>
    /// <param name="index">位置索引</param>
    /// <returns>返回元素的const引用</returns>
    /// <exception cref="std::out_of_range">当index超出范围时抛出</exception>
    constexpr const Type& At(size_type index) const
    {
        checkf(IsValidIndex(index));

        return m_data[index];
    }
    /// <summary>
    /// 访问第一个元素
    /// </summary>
    /// <returns>返回首元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    Type& Front()
    {
        checkf(!IsEmpty());

        return *m_data;
    }
    /// <summary>
    /// 访问第一个元素(const版本)
    /// </summary>
    /// <returns>返回首元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr const Type& Front() const
    {
        checkf(!IsEmpty());

        return *m_data;
    }
    /// <summary>
    /// 访问最后一个元素
    /// </summary>
    /// <returns>返回尾元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr Type& Back()
    {
        checkf(!IsEmpty());

        return m_data[m_size - 1];
    }
    /// <summary>
    /// 访问最后一个元素(const版本)
    /// </summary>
    /// <returns>返回尾元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr const Type& Back() const
    {
        checkf(!IsEmpty());

        return m_data[m_size - 1];
    }
    /// <summary>
    /// 获取底层容器指针
    /// </summary>
    /// <returns>指向容器首元素的指针</returns>
    constexpr Type* Data()
    {
        return m_data;
    }
    /// <summary>
    /// 获取底层容器指针(const版本)
    /// </summary>
    /// <returns>指向容器首元素的const指针</returns>
    constexpr const Type* Data() const
    {
        return m_data;
    }
    /// <summary>
    /// 获取容器当前元素数量
    /// </summary>
    /// <returns>元素数量</returns>
    constexpr size_type Size() const
    {
        return m_size;
    }
    /// <summary>
    /// 获取容器能够容纳的最大元素数量
    /// </summary>
    /// <returns>最大元素数量</returns>
    constexpr size_type MaxSize() const
    {
        return std::numeric_limits<size_type>::max();
    }
    /// <summary>
    /// 获取容器字节大小
    /// </summary>
    /// <returns>字节大小</returns>
    constexpr size_type ByteSize() const
    {
        return m_size * sizeof(Type);
    }
    /// <summary>
    /// 获取容器当前容量
    /// </summary>
    /// <returns>当前容量</returns>
    constexpr size_type Capacity() const
    {
        return m_capacity;
    }
    /// <summary>
    /// 获取容器当前剩余容量
    /// </summary>
    /// <returns>剩余容量</returns>
    constexpr size_type Remain() const
    {
        return m_capacity - m_size;
    }
    /// <summary>
    /// 清空容器(元素被销毁，容量保持不变)
    /// </summary>
    constexpr void Clear()
    {
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        m_size = 0;
    }
    /// <summary>
    /// 重置容器(元素被销毁，容量变为 0)
    /// </summary>
    constexpr void Reset()
    {
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        m_size = 0;
        if (m_capacity != 0)
        {
            m_alloc.Deallocate(m_data, m_capacity);
        }
        m_capacity = 0;
    }
    /// <summary>
    /// 预留存储空间(避免多次重新分配)
    /// </summary>
    /// <param name="size">期望的最小容量</param>
    constexpr void Reserve(size_type size)
    {
        // 如果请求容量小于等于当前容量，直接返回
        if (size <= m_capacity) return;

        // 定义常量参数
        constexpr size_type init_capacity = 16; // 默认初始容量（2的幂）
        constexpr size_type growth_factor = 3;   // 指数增长因子(1.5 * 2 避免浮点运算)
        constexpr size_type growth_threshold = 256; // 增长阈值
        constexpr size_type growth_amount = 64; // 固定增长量

        size_type new_capacity = size; // 实际应当分配的容量

        // 初始容量策略：避免过小容量导致的频繁分配
        if (m_capacity == 0)
        {
            new_capacity = std::max(size, init_capacity);
        }
        // 小容量策略（当前容量 < 阈值）：使用1.5倍增长
        else if (m_capacity < growth_threshold)
        {
            // 使用整数运算计算1.5倍增长
            new_capacity = m_capacity + (m_capacity + 1) / growth_factor;
            // 确保至少增长一个元素
            if (new_capacity <= m_capacity)
            {
                new_capacity = m_capacity + 1;
            }
            // 确保不小于请求容量
            if (new_capacity < size)
            {
                new_capacity = size;
            }
        }
        // 大容量策略（当前容量 >= 阈值）：使用固定大小增长
        else
        {
            // 固定大小增长容量
            new_capacity = m_capacity + growth_amount;
            // 确保不小于请求容量
            if (new_capacity < size)
            {
                new_capacity = size;
            }
        }

        // 分配新内存
        Type* new_data = m_alloc.Allocate<Type>(new_capacity);

        // 移动或复制元素到新内存
        if (m_size > 0)
        {
            for (size_type i = 0; i < m_size; i++)
            {
                std::construct_at(&new_data[i], std::move_if_noexcept(m_data[i]));
            }
        }

        // 销毁旧元素并释放旧内存
        if (m_capacity != 0)
        {
            for (size_type i = 0; i < m_size; i++)
            {
                std::destroy_at(&m_data[i]);
            }
            m_alloc.Deallocate(m_data, m_capacity);
        }

        // 更新指针和容量
        m_data = new_data;
        m_capacity = new_capacity;
    }
    /// <summary>
    /// 调整容器大小
    /// </summary>
    /// <param name="size">新的容器大小</param>
    constexpr void Resize(size_type size)
    {
        if (size < m_size)
        {
            // 销毁多余元素
            for (size_type i = size; i != m_size; i++)
            {
                std::destroy_at(&m_data[i]);
            }
            m_size = size;
        }
        else if (size > m_size)
        {
            // 预留空间并构造新元素
            Reserve(size);
            for (size_type i = m_size; i != size; i++)
            {
                std::construct_at(&m_data[i]);
            }
            m_size = size; // 仅在扩容时更新m_size
        }
    }
    /// <summary>
    /// 释放未使用的内存
    /// </summary>
    constexpr void Shrink()
    {
        if (m_size == m_capacity) return;
        auto old_data = m_data;
        auto old_capacity = m_capacity;
        m_capacity = m_size;
        if (m_size == 0)
        {
            m_data = nullptr;
        }
        else
        {
            m_data = m_alloc.Allocate(m_size);
        }
        if (old_capacity != 0) [[likely]]
        {
            for (size_type i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
                std::destroy_at(&old_data[i]);
            }
            m_alloc.Deallocate(old_data, old_capacity);
        }
    }
    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(const Type& value)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], value);
        ++m_size;
        return iterator(m_data + m_size - 1);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(Type&& value)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], std::move(value));
        ++m_size;
        return iterator(m_data + m_size - 1);
    }
    /// <summary>
    /// 在容器末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], std::forward<Args>(args)...);
        ++m_size;
        return iterator(m_data + m_size - 1);
    }
    /// <summary>
    /// 在指定位置就地构造一个元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="args">构造元素的参数</param>
    /// <returns></returns>
    template<class... Args>
    constexpr iterator Emplace(const_iterator iter, Args&&... args)
    {
        const size_type index = iter - cbegin();
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        // 移动元素以腾出位置
        for (size_type i = m_size; i > index; --i)
        {
            std::construct_at(&m_data[i], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        // 就地构造新元素
        std::construct_at(&m_data[index], std::forward<Args>(args)...);
        ++m_size;
        return iterator(m_data + index);
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr Array& Append(const Array& other)
    {
        // 处理自赋值:创建临时副本
        if (this == &other)
        {
            Array temp(other);
            return Append(temp);
        }

        // 空容器处理
        if (other.m_size == 0)
        {
            return *this;
        }

        const size_type new_size = m_size + other.Size();
        Reserve(new_size);

        // 优化：如果元素平凡可复制，使用内存拷贝
        if constexpr (std::is_trivially_copyable_v<Type>)
        {
            std::memcpy(m_data + m_size, other.m_data, other.m_size * sizeof(Type));
            m_size = new_size;
        }
        else
        {
            for (size_type i = 0; i < other.Size(); ++i)
            {
                Push(other.m_data[i]);
            }
        }

        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr Array& Append(Array&& other) noexcept
    {
        // 处理自移动:直接返回
        if (this == &other)
        {
            return *this;
        }

        // 空容器处理
        if (other.m_size == 0) {
            return *this;
        }

        const size_type new_size = m_size + other.Size();
        Reserve(new_size);

        // 优化：平凡类型使用 memcpy
        if constexpr (std::is_trivially_copyable_v<Type>)
        {
            std::memcpy(m_data + m_size, other.m_data, other.m_size * sizeof(Type));
            m_size = new_size;
            other.m_size = 0; // 重置 other 的大小
        }
        // 优化：可移动类型批量移动
        else if constexpr (std::is_nothrow_move_constructible_v<Type>)
        {
            std::uninitialized_move_n(other.m_data, other.m_size, m_data + m_size);
            m_size = new_size;
            other.m_size = 0;
        }
        // 通用：逐个移动元素
        else
        {
            for (size_type i = 0; i < other.Size(); ++i)
            {
                Push(std::move(other.m_data[i]));
            }
            other.m_size = 0;
        }
        return *this;
    }
    /// <summary>
    /// 在容器末尾追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <class InputIt>
    constexpr Array& Append(InputIt first, InputIt last)
    {
        // 计算要添加的元素数量
        const size_type count = last - first;

        // 空容器处理
        if (count == 0)
        {
            return *this;
        }

        // 检查并处理自引用（迭代器指向当前容器）
        if constexpr (std::is_pointer_v<InputIt>)
        {
            const auto* first_ptr = std::to_address(first);
            const auto* last_ptr = std::to_address(last);
            const bool is_self_ref = (first_ptr >= m_data) && (first_ptr < m_data + m_size);

            if (is_self_ref)
            {
                Array temp;
                temp.Reserve(count);
                for (auto iter = first; iter != last; ++iter)
                {
                    temp.Push(*iter);
                }
                return Append(temp);
            }
        }

        // 预留足够空间
        const size_t new_size = m_size + count;
        Reserve(new_size);

        // 批量添加元素（根据类型选择最优方式）
        if constexpr (std::is_trivially_copyable_v<Type>)
        {
            // 平凡类型：使用内存拷贝
            std::memcpy(m_data + m_size, std::to_address(first), count * sizeof(Type));
            m_size = new_size;
        }
        else
        {
            // 非平凡类型：使用 uninitialized_copy
            std::uninitialized_copy(first, last, m_data + m_size);
            m_size = new_size;
        }

        return *this;
    }
    /// <summary>
    /// 在容器末尾追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& Append(std::initializer_list<Type> ilist)
    {
        return Append(ilist.begin(), ilist.end());
    }
    /// <summary>
    /// 在起始追加另一个容器的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr Array& Prepend(const Array& other)
    {
        // 处理自赋值:创建临时副本，避免自引用导致的元素覆盖
        if (this == &other)
        {
            Array temp(other);
            return Prepend(temp);
        }

        // 空容器处理，直接返回
        if (other.m_size == 0)
        {
            return *this;
        }

        const size_type new_size = m_size + other.Size();
        const size_type prepend_count = other.Size();

        // 预留足够空间
        Reserve(new_size);

        // 从后往前移动，避免元素覆盖
        for (size_type i = m_size - 1; i >= 0; --i)
        {
            std::construct_at(&m_data[i + prepend_count], std::move_if_noexcept(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 拷贝other的元素到头部
        if constexpr (std::is_trivially_copyable_v<Type>)
        {
            // 平凡可复制类型，使用内存拷贝优化
            std::memcpy(m_data, other.m_data, prepend_count * sizeof(Type));
        }
        else
        {
            // 非平凡类型，逐个拷贝构造
            for (size_type i = 0; i < prepend_count; ++i)
            {
                std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
            }
        }

        // 更新元素数量
        m_size = new_size;
        return *this;
    }
    /// <summary>
    /// 在起始追加另一个容器的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr Array& Prepend(Array&& other) noexcept
    {
        // 处理自移动:直接返回，避免资源混乱
        if (this == &other)
        {
            return *this;
        }

        // 空容器处理，直接返回
        if (other.m_size == 0)
        {
            return *this;
        }

        const size_type new_size = m_size + other.Size();
        const size_type prepend_count = other.Size();

        // 预留足够空间
        Reserve(new_size);

        // 移动现有元素，为前置元素腾出头部空间
        for (size_type i = m_size - 1; i >= 0; --i)
        {
            std::construct_at(&m_data[i + prepend_count], std::move_if_noexcept(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 移动other的元素到头部（避免拷贝，提升性能）
        if constexpr (std::is_trivially_move_constructible_v<Type> && std::is_trivially_copyable_v<Type>)
        {
            // 平凡类型，使用内存拷贝优化（移动等价于拷贝）
            std::memcpy(m_data, other.m_data, prepend_count * sizeof(Type));
        }
        else if constexpr (std::is_nothrow_move_constructible_v<Type>)
        {
            // 可 noexcept 移动的类型，批量移动
            std::uninitialized_move_n(other.m_data, prepend_count, m_data);
        }
        else
        {
            // 通用情况，逐个移动构造
            for (size_type i = 0; i < prepend_count; ++i)
            {
                std::construct_at(&m_data[i], std::move(other.m_data[i]));
            }
        }

        // 更新当前容器大小
        m_size = new_size;

        // 重置other，使其处于空容器状态
        other.m_size = 0;
        return *this;
    }
    /// <summary>
    /// 在容器起始追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <class InputIt>
    constexpr Array& Prepend(InputIt first, InputIt last)
    {
        // 计算要前置的元素数量
        const size_type count = last - first;

        // 空范围处理，直接返回
        if (count == 0)
        {
            return *this;
        }

        // 检查并处理自引用（迭代器指向当前容器）
        if constexpr (std::is_pointer_v<InputIt>)
        {
            const auto* first_ptr = std::to_address(first);
            const auto* last_ptr = std::to_address(last);
            const bool is_self_ref = (first_ptr >= m_data) && (first_ptr < m_data + m_size);

            if (is_self_ref)
            {
                // 自引用时，先创建临时副本
                Array temp(first, last);
                return Prepend(temp);
            }
        }

        const size_type new_size = m_size + count;

        // 预留足够空间
        Reserve(new_size);

        // 移动现有元素，腾出头部空间
        for (size_type i = m_size - 1; i >= 0; --i)
        {
            std::construct_at(&m_data[i + count], std::move_if_noexcept(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 将迭代器范围的元素添加到头部
        if constexpr (std::is_trivially_copyable_v<Type>)
        {
            // 平凡类型，内存拷贝优化
            std::memcpy(m_data, std::to_address(first), count * sizeof(Type));
        }
        else
        {
            // 非平凡类型，逐个拷贝构造
            InputIt iter = first;
            for (size_type i = 0; i < count; ++i, ++iter)
            {
                std::construct_at(&m_data[i], *iter);
            }
        }

        // 更新元素数量
        m_size = new_size;
        return *this;
    }
    /// <summary>
    /// 在容器起始追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& Prepend(std::initializer_list<Type> ilist)
    {
        return Prepend(ilist.begin(), ilist.end());
    }
    /// <summary>
    /// 在指定位置插入一个元素(拷贝语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, const Type& value)
    {
        return Insert(iter, 1, value);
    }
    /// <summary>
    /// 在指定位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, Type&& value)
    {
        const ptrdiff_t index = iter - cbegin();

        if (index < 0 || index > m_size)
        {
            throw std::out_of_range("Iterator out of range");
        }

        if (m_size + 1 > m_capacity)
        {
            Reserve(m_size + 1);
        }

        // 移动元素腾出空间
        for (size_type i = m_size; i > index; --i)
        {
            std::construct_at(&m_data[i], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }

        // 构造新元素
        std::construct_at(&m_data[index], std::move(value));
        ++m_size;
        return begin() + index;
    }
    /// <summary>
    /// 在指定位置插入 count 个相同元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="count">要插入的元素数量</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, size_type count, const Type& value)
    {
        const size_type index = iter - cbegin();
        if (index < 0 || index > m_size || count < 0)
        {
            throw std::out_of_range("Invalid insert position or count");
        }

        if (count == 0)
        {
            return begin() + index;
        }

        const size_type new_size = m_size + count;

        if (new_size > m_capacity)
        {
            Reserve(new_size);
        }

        // 移动现有元素
        for (size_type i = m_size - 1; i >= index; --i)
        {
            std::construct_at(&m_data[i + count], std::move(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 构造新元素
        for (size_type i = 0; i < count; ++i)
        {
            std::construct_at(&m_data[index + i], value);
        }

        m_size = new_size;
        return begin() + index;
    }
    /// <summary>
    /// 在指定位置插入从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    template<class InputIt>
    constexpr iterator Insert(const_iterator iter, InputIt first, InputIt last)
    {
        const size_type index = iter - cbegin();
        const size_type count = last - first;

        if (count <= 0)
        {
            return begin() + index;
        }

        if (index < 0 || index > m_size)
        {
            throw std::out_of_range("Iterator out of range");
        }

        const size_type new_size = m_size + count;

        if (new_size > m_capacity)
        {
            Reserve(new_size);
        }

        // 移动现有元素
        for (size_type i = m_size - 1; i >= index; --i)
        {
            std::construct_at(&m_data[i + count], std::move(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 复制新元素
        for (size_type i = 0; i < count; ++i, ++first)
        {
            std::construct_at(&m_data[index + i], *first);
        }

        m_size = new_size;
        return begin() + index;
    }
    /// <summary>
    /// 在指定位置插入初始化列表中的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="ilist">初始化列表</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, std::initializer_list<Type> ilist)
    {
        return Insert(iter, ilist.begin(), ilist.end());
    }
    /// <summary>
    /// 在指定索引位置插入一个元素(拷贝语义)
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(size_type index, const Type& value)
    {
        Insert(cbegin() + index, value);
    }
    /// <summary>
    /// 在指定索引位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(size_type index, Type&& value)
    {
        Insert(cbegin() + index, std::move(value));
    }
    /// <summary>
    /// 在指定索引位置插入 count 个相同元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="count">要插入的元素数量</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(size_type index, size_type count, const Type& value)
    {
        Insert(cbegin() + index, count, value);
    }
    /// <summary>
    /// 在指定索引位置插入从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template<class InputIt>
    constexpr void Insert(size_type index, InputIt first, InputIt last)
    {
        Insert(cbegin() + index, first, last);
    }
    /// <summary>
    /// 在指定索引位置插入初始化列表中的元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="ilist">初始化列表</param>
    constexpr void Insert(size_type index, std::initializer_list<Type> ilist)
    {
        Insert(cbegin() + index, ilist);
    }
    /// <summary>
    /// 移除指定位置的元素
    /// </summary>
    /// <param name="iter">要移除元素的迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator iter)
    {
        return Erase(iter, iter + 1);
    }
    /// <summary>
    /// 移除从 firstIter 到 lastIter 范围内的元素
    /// </summary>
    /// <param name="firstIter">起始迭代器</param>
    /// <param name="lastIter">结束迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        const ptrdiff_t first = firstIter - cbegin();
        const ptrdiff_t last = lastIter - cbegin();
        const ptrdiff_t count = last - first;

        if (count <= 0) return begin() + last;
        if (first < 0 || last > m_size) {
            throw std::out_of_range("Erase range out of bounds");
        }

        // 销毁范围内的元素
        for (ptrdiff_t i = first; i < last; ++i) {
            std::destroy_at(&m_data[i]);
        }

        // 移动后续元素
        for (ptrdiff_t i = last; i < m_size; ++i) {
            std::construct_at(&m_data[i - count], std::move(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        m_size -= static_cast<size_type>(count);
        return begin() + first;
    }
    /// <summary>
    /// 移除指定索引位置的元素
    /// </summary>
    /// <param name="index">要移除元素的索引</param>
    constexpr void Erase(size_type index)
    {
        Erase(cbegin() + index);
    }
    /// <summary>
    /// 移除从 start 到 end 索引范围内的元素
    /// </summary>
    /// <param name="start">起始索引</param>
    /// <param name="end">结束索引</param>
    constexpr void Erase(size_type start, size_type end)
    {
        Erase(cbegin() + start, cbegin() + end);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(const Type& value)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], value);
        m_size++;
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(Type&& value)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], std::move(value));
        m_size++;
    }
    /// <summary>
    /// 移除容器末尾的元素
    /// </summary>
    constexpr void Pop()
    {
        if (m_size == 0) return;
        std::destroy_at(&m_data[m_size - 1]);
        --m_size;
    }
    /// <summary>
    /// 在容器开头添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(const Type& value)
    {
        Insert(cbegin(), value);
    }
    /// <summary>
    /// 在容器开头添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(Type&& value)
    {
        Insert(cbegin(), std::move(value));
    }
    /// <summary>
    /// 移除容器开头的元素
    /// </summary>
    constexpr void Shift()
    {
        if (m_size == 0) return;
        Erase(cbegin());
    }
    /// <summary>
    /// 检查容器中是否包含指定值
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>如果找到返回 true，否则返回 false</returns>
    constexpr bool Contains(const Type& value)
    {
        for (size_type i = 0; i < m_size; ++i)
        {
            if (m_data[i] == value)
            {
                return true;
            }
        }
        return false;
    }
    /// <summary>
    /// 查找第一个满足条件的元素
    /// </summary>
    /// <param name="value">指定元素</param>
    /// <returns>指向匹配元素的迭代器</returns>
    constexpr iterator Find(const Type& value)
    {
        for (iterator iter = begin(); iter != end(); ++iter)
        {
            if (*iter == value)
            {
                return iter;
            }
        }
        return end();
    }
    /// <summary>
    /// 查找最后一个满足条件的元素
    /// </summary>
    /// <param name="value">指定元素</param>
    /// <returns>指向匹配元素的迭代器</returns>
    constexpr iterator FindLast(const Type& value)
    {
        for (auto iter = rbegin(); iter != rend(); ++iter)
        {
            if (*iter == value)
            {
                return (iter + 1).base();
            }
        }
        return end();
    }
    /// <summary>
    /// 查找指定值的第一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr size_type IndexOf(const Type& value, size_type start = 0)
    {
        const size_type size = Size();
        if (start < 0) start = 0;

        for (size_type i = start; i < size; ++i)
        {
            if ((*this)[i] == value)
            {
                return i;
            }
        }
        return -1;
    }
    /// <summary>
    /// 查找指定值的最后一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr size_type LastIndexOf(const Type& value, size_type start = -1)
    {
        const size_type size = Size();
        if (size == 0) return -1;

        if (start < 0 || start >= size)
        {
            start = size - 1;
        }

        for (size_type i = start; i >= 0; --i)
        {
            if ((*this)[i] == value)
            {
                return i;
            }
        }
        return -1;
    }
    /// <summary>
    /// 反转元素的顺序
    /// </summary>
    void Reverse()
    {
        // 空容器或只有一个元素，无需反转
        if (m_size <= 1)
        {
            return;
        }

        // 双指针法：首尾交换，向中间靠拢
        size_type left = 0;
        size_type right = m_size - 1;

        while (left < right)
        {
            // 优化：平凡可交换类型直接交换，非平凡类型移动交换
            if constexpr (std::is_trivially_copyable_v<Type>)
            {
                std::swap(m_data[left], m_data[right]);
            }
            else
            {
                // 使用移动语义避免拷贝，提升效率
                Type temp = std::move(m_data[left]);
                m_data[left] = std::move(m_data[right]);
                m_data[right] = std::move(temp);
            }
            ++left;
            --right;
        }
    }
    /// <summary>
    /// 检查容器是否为空
    /// </summary>
    /// <returns>true表示容器为空</returns>
    constexpr bool IsEmpty() const
    {
        return m_size == 0;
    }
    /// <summary>
    /// 检查索引是否有效
    /// </summary>
    /// <param name="index">要检查的索引</param>
    /// <returns>如果索引有效返回 true，否则返回 false</returns>
    constexpr bool IsValidIndex(size_type index) const
    {
        return index >= 0 && index < m_size;
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    constexpr void Swap(Array& other)
    {
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_alloc, other.m_alloc);
        std::swap(m_data, other.m_data);
    }
public:
    /// <summary>
    /// 下标运算符
    /// </summary>
    constexpr Type& operator[](size_type index)
    {
        return m_data[index];
    }
    /// <summary>
    /// 下标运算符（const版本）
    /// </summary>
    constexpr const Type& operator[](size_type index) const
    {
        return m_data[index];
    }
    /// <summary>
    /// 加法运算符
    /// </summary>
    constexpr friend Array operator+(const Array& left, const Array& right)
    {
        Array<Type> result;
        result.Reserve(left.Size() + right.Size());
        result.Append(left);
        result.Append(right);
        return result;
    }
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    constexpr Array& operator+=(const Array& other)
    {
        return Append(other);
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const Array& left, const Array& right)
    {
        if (left.m_size != right.m_size)
        {
            return false;
        }

        for (size_type i = 0; i < left.m_size; ++i)
        {
            if (left.m_data[i] != right.m_data[i])
            {
                return false;
            }
        }
        return true;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const Array& left, const Array& right)
    {
        if (left.m_size != right.m_size)
        {
            return true;
        }

        for (size_type i = 0; i < left.m_size; ++i)
        {
            if (left.m_data[i] != right.m_data[i])
            {
                return true;
            }
        }
        return false;
    }
    /// <summary>
    /// 小于运算符
    /// </summary>
    constexpr friend bool operator<(const Array& left, const Array& right)
    {
        const size_type minSize = std::min(left.m_size, right.m_size);

        for (size_type i = 0; i < minSize; ++i)
        {
            if (left.m_data[i] < right.m_data[i])
            {
                return true;
            }
            if (left.m_data[i] > right.m_data[i])
            {
                return false;
            }
        }

        // 所有比较的元素都相等，比较长度
        return left.m_size < right.m_size;
    }
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    constexpr friend bool operator<=(const Array& left, const Array& right)
    {
        return !(right < left);
    }
    /// <summary>
    /// 大于运算符
    /// </summary>
    constexpr friend bool operator>(const Array& left, const Array& right)
    {
        return right < left;
    }
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    constexpr friend bool operator>=(const Array& left, const Array& right)
    {
        return !(left < right);
    }
public:
    /// <summary>
    /// 返回指向第一个元素的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return iterator(m_data);
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return const_iterator(m_data);
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return const_iterator(m_data);
    }
    /// <summary>
    /// 返回指向末尾的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return iterator(m_data + m_size);
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return const_iterator(m_data + m_size);
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return const_iterator(m_data + m_size);
    }
    /// <summary>
    /// 返回指向最后一个元素的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }  
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
private:
    size_type m_size = 0;
    size_type m_capacity = 0;
    Allocator m_alloc = Allocator();
    Type* m_data = nullptr;
};