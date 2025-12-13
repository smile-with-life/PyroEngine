#pragma once

#include "Core.h"
#include "Concept/Concept.h"
#include "Iterator/Iterator.h"
#include "Allocator/Allocator.h"

#include <utility>
#include <type_traits>


template <class Type>
class ArrayIterator
{
public:
    using category = ContiguousIteratorTag;
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
    [[nodiscard]] constexpr Type& operator*() const noexcept
    {
        return *m_ptr;
    }

    [[nodiscard]] constexpr Type* operator->() const noexcept
    {
        return m_ptr;
    }

    [[nodiscard]] constexpr Type& operator[](const ptrdiff off) const noexcept
    {
        return *(*this + off);
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

    constexpr ArrayIterator& operator+=(const ptrdiff off) noexcept
    {
        m_ptr += off;
        return *this;
    }

    constexpr ArrayIterator& operator-=(const ptrdiff off) noexcept
    {
        return *this += -off;
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

    [[nodiscard]] constexpr bool operator>(const ArrayIterator& right) const noexcept
    {
        return right < *this;
    }

    [[nodiscard]] constexpr bool operator<=(const ArrayIterator& right) const noexcept
    {
        return !(right < *this);
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
    using category = ContiguousIteratorTag;
    using value_type = Type;
    using reference = Type&;
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
    [[nodiscard]] constexpr const Type& operator*() const noexcept
    {
        return *m_ptr;
    }

    [[nodiscard]] constexpr const Type* operator->() const noexcept
    {
        return m_ptr;
    }

    [[nodiscard]] constexpr const Type& operator[](const ptrdiff off) const noexcept
    {
        return *(*this + off);
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

    constexpr ArrayConstIterator& operator+=(const ptrdiff off) noexcept
    {
        m_ptr += off;
        return *this;
    }

    constexpr ArrayConstIterator& operator-=(const ptrdiff off) noexcept
    {
        return *this += -off;
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

    [[nodiscard]] constexpr bool operator>(const ArrayConstIterator& right) const noexcept
    {
        return right < *this;
    }

    [[nodiscard]] constexpr bool operator<=(const ArrayConstIterator& right) const noexcept
    {
        return !(right < *this);
    }

    [[nodiscard]] constexpr bool operator>=(const ArrayConstIterator& right) const noexcept
    {
        return !(*this < right);
    }
private:
    Type* m_ptr;

};

template<class Type>
class Array 
{
public:
    using iterator = ArrayIterator<Type>;
    using const_iterator = ArrayConstIterator<Type>;
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
        Clear();
        if (m_capacity != 0)
        {
            m_alloc.Deallocate(m_data, m_capacity);
        }
    }
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的数组</param>
    constexpr Array(const Array& other)
        : m_alloc(other.m_alloc)
    {
        m_capacity = m_size = other.m_size;
        if (m_size != 0)
        {
            m_data = m_alloc.Allocate(m_capacity);
            for (int32 i = 0; i != m_size; i++)
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
    /// <param name="other">要拷贝的数组</param>
    /// <returns>this</returns>
    constexpr Array& operator=(const Array& other)
    {
        if (&other == this) [[unlikely]]
        {
            return *this;
        }    
        Reserve(other.m_size);
        m_size = other.m_size;
        for (int32 i = 0; i != m_size; i++)
        {
            std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
        }
        return *this;
    }
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的数组</param>
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
    /// <param name="other">要移动的数组</param>
    /// <returns>this</returns>
    constexpr Array& operator=(Array&& other)
    {
        // 判断自赋值
        if (&other == this) [[unlikely]]
        {
            return *this;
        }
        // 销毁元素
        for (int32 i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        // 释放内存
        if (m_capacity != 0)
        {
            m_alloc.Deallocate(m_data, m_capacity);
        }
        // 接管 other 的资源
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_alloc = other.m_alloc;
        // 将 other 置为空数组 
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }
    /// <summary>
    /// 构造函数，创建指定大小的数组
    /// </summary>
    /// <param name="count">数组初始大小</param>
    constexpr explicit Array(int32 count)
    {
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (int32 i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i]);
        }
    }
    /// <summary>
    /// 构造函数，指定数组大小并赋初值
    /// </summary>
    /// <param name="count">数组初始大小</param>
    /// <param name="value">用于填充的初始值</param>
    constexpr Array(int32 count, const Type& value)
    {
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (int32 i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], value);
        }
    }
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template <std::random_access_iterator InputIt>
    constexpr Array(InputIt first, InputIt last)
    {
        int32 count = last - first;
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (int32 i = 0; i != count; i++)
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
        int32 count = last - first;
        m_data = m_alloc.Allocate(count);
        m_capacity = m_size = count;
        for (int32 i = 0; i != count; i++)
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
    constexpr Type& At(int32 index)
    {
        if (index >= m_size) [[unlikely]] 
        {
            throw std::out_of_range("Array::At " + std::to_string(index) + " out of range");
        }
        return m_data[index];
    }
    /// <summary>
    /// 访问指定位置的元素(const版本)
    /// </summary>
    /// <param name="index">位置索引</param>
    /// <returns>返回元素的const引用</returns>
    /// <exception cref="std::out_of_range">当index超出范围时抛出</exception>
    constexpr const Type& At(int32 index) const
    {
        if (index >= m_size) [[unlikely]]
        {
            throw std::out_of_range("Array::At " + std::to_string(index) + " out of range");
        }
        return m_data[index];
    }
    /// <summary>
    /// 访问第一个元素
    /// </summary>
    /// <returns>返回首元素的引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    Type& Front()
    {
        if (m_size == 0) [[unlikely]]
        {
            throw std::out_of_range("The array is empty");
        }
        return *m_data;
    }
    /// <summary>
    /// 访问第一个元素(const版本)
    /// </summary>
    /// <returns>返回首元素的const引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr const Type& Front() const
    {
        if (m_size == 0) [[unlikely]]
        {
            throw std::out_of_range("The array is empty");
        }
        return *m_data;
    }
    /// <summary>
    /// 访问最后一个元素
    /// </summary>
    /// <returns>返回尾元素的引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr Type& Back()
    {
        if (m_size == 0) [[unlikely]]
        {
            throw std::out_of_range("The array is empty");
        }
        return m_data[m_size - 1];
    }
    /// <summary>
    /// 访问最后一个元素(const版本)
    /// </summary>
    /// <returns>返回尾元素的const引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr const Type& Back() const
    {
        if (m_size == 0) [[unlikely]]
        {
            throw std::out_of_range("The array is empty");
        }
        return m_data[m_size - 1];
    }
    /// <summary>
    /// 获取底层数组指针
    /// </summary>
    /// <returns>指向数组首元素的指针</returns>
    constexpr Type* Data()
    {
        return m_data;
    }
    /// <summary>
    /// 获取底层数组指针(const版本)
    /// </summary>
    /// <returns>指向数组首元素的const指针</returns>
    constexpr const Type* Data() const
    {
        return m_data;
    }
    /// <summary>
    /// 获取数组当前已使用的内存字节大小
    /// </summary>
    /// <returns>已使用的内存字节大小</returns>
    constexpr uint64 ByteSize() const
    {
        return m_size * sizeof(Type);
    }
    /// <summary>
    /// 获取模板参数类型Type的字节大小
    /// </summary>
    /// <returns>类型Type的字节大小</returns>
    constexpr int32 TypeSize()
    {
        return sizeof(Type);
    }
    /// <summary>
    /// 获取数组当前元素数量
    /// </summary>
    /// <returns>元素数量</returns>
    constexpr int32 Size() const
    {
        return m_size;
    }
    /// <summary>
    /// 获取容器能够容纳的最大元素数量
    /// </summary>
    /// <returns>最大元素数量</returns>
    int32 Max() const
    {
        return std::numeric_limits<int32>::max();
    }
    /// <summary>
    /// 获取数组当前容量
    /// </summary>
    /// <returns>当前容量</returns>
    constexpr int32 Capacity() const
    {
        return m_capacity;
    }
    /// <summary>
    /// 清空数组(元素被销毁，容量保持不变)
    /// </summary>
    constexpr void Clear()
    {
        for (int32 i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        m_size = 0;
    }
    /// <summary>
    /// 预留存储空间(避免多次重新分配)
    /// </summary>
    /// <param name="size">期望的最小容量</param>
    constexpr void Reserve(int32 size)
    {
        // 如果请求容量小于等于当前容量，直接返回
        if (size <= m_capacity) return;

        // 定义常量参数
        constexpr int32 init_capacity = 16; // 默认初始容量（2的幂）
        constexpr int32 growth_factor = 3;   // 指数增长因子(1.5 * 2 避免浮点运算)
        constexpr int32 growth_threshold = 256; // 增长阈值
        constexpr int32 growth_amount = 64; // 固定增长量

        int32 new_capacity = size; // 实际应当分配的容量

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
            for (int32 i = 0; i < m_size; i++)
            {
                std::construct_at(&new_data[i], std::move_if_noexcept(m_data[i]));
            }
        }

        // 销毁旧元素并释放旧内存
        if (m_capacity != 0) 
        {
            for (int32 i = 0; i < m_size; i++) 
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
    /// 调整数组大小
    /// </summary>
    /// <param name="size">新的数组大小</param>
    constexpr void Resize(int32 size)
    {
        if (size < m_size)
        {
            for (int32 i = size; i != m_size; i++)
            {
                std::destroy_at(&m_data[i]);
            }
            m_size = size;
        }
        else if (size > m_size)
        {
            Reserve(size);
            for (int32 i = m_size; i != size; i++)
            {
                std::construct_at(&m_data[i]);
            }
        }
        m_size = size;
    }
    /// <summary>
    /// 释放未使用的内存
    /// </summary>
    constexpr void Shrink()
    {
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
            for (int32 i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
                std::destroy_at(&old_data[i]);
            }
            m_alloc.Deallocate(old_data, old_capacity);
        }
    }
    /// <summary>
    /// 在末尾添加元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Add(const Type& value)
    {
        if (m_size + 1 > m_capacity) [[unlikely]]
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], value);
        m_size++;
        return *this;
    }
    /// <summary>
    /// 在末尾添加元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Add(Type&& value) noexcept
    {
        if (m_size + 1 > m_capacity) [[unlikely]] 
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], std::move(value));
        m_size++;
        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个数组的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(const Array& other)
    {
        // 处理自赋值:创建临时副本
        if (this == &other) 
        {
            Array temp(other);
            return Append(temp);
        }

        // 空数组处理
        if (other.m_size == 0) 
        { 
            return *this; 
        }

        const int32 new_size = m_size + other.Size();
        Reserve(new_size);

        // 优化：如果元素平凡可复制，使用内存拷贝
        if constexpr (std::is_trivially_copyable_v<Type>) 
        {
            std::memcpy(m_data + m_size, other.m_data, other.m_size * sizeof(Type));
            m_size = new_size;
        }
        else 
        {
            for (int32 i = 0; i < other.Size(); ++i)
            {
                Add(other.m_data[i]);
            }
        }

        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个数组的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(Array&& other) noexcept
    {
        // 处理自移动:直接返回
        if (this == &other) 
        {
            return *this;
        }

        // 空数组处理
        if (other.m_size == 0) {
            return *this;
        }

        const int32 new_size = m_size + other.Size();
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
            for (int32 i = 0; i < other.Size(); ++i) 
            {
                Add(std::move(other.m_data[i]));
            }
            other.m_size = 0;
        }
        return *this;
    }
    /// <summary>
    /// 在数组末尾追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <std::random_access_iterator InputIt>
    constexpr Array& Append(InputIt first, InputIt last)
    {
        // 计算要添加的元素数量
        const int32 count = last - first;

        // 空数组处理
        if (count == 0)
        {
            return *this;
        }

        // 检查并处理自引用（迭代器指向当前数组）
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
                    temp.Add(*iter);
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
    /// 在数组末尾追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& Append(std::initializer_list<Type> ilist)
    {
        return Append(ilist.begin(), ilist.end());
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
        for (int32 i = m_size; i > index; --i) 
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
    constexpr iterator Insert(const_iterator iter, int32 count, const Type& value)
    {
        const int32 index = iter - cbegin();
        if (index < 0 || index > m_size || count < 0) 
        {
            throw std::out_of_range("Invalid insert position or count");
        }

        if (count == 0)
        {
            return begin() + index;
        }         

        const int32 new_size = m_size + count;

        if (new_size > m_capacity) 
        {
            Reserve(new_size);
        }

        // 移动现有元素
        for (int32 i = m_size - 1; i >= index; --i) 
        {
            std::construct_at(&m_data[i + count], std::move(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 构造新元素
        for (int32 i = 0; i < count; ++i) 
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
        const int32 index = iter - cbegin();
        const int32 count = last - first;

        if (count <= 0)
        {
            return begin() + index;
        }

        if (index < 0 || index > m_size) 
        {
            throw std::out_of_range("Iterator out of range");
        }

        const int32 new_size = m_size + count;

        if (new_size > m_capacity) 
        {
            Reserve(new_size);
        }

        // 移动现有元素
        for (int32 i = m_size - 1; i >= index; --i) 
        {
            std::construct_at(&m_data[i + count], std::move(m_data[i]));
            std::destroy_at(&m_data[i]);
        }

        // 复制新元素
        for (int32 i = 0; i < count; ++i, ++first) 
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
    constexpr void Insert(int32 index, const Type& value)
    {
        Insert(cbegin() + index, value);
    }
    /// <summary>
    /// 在指定索引位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(int32 index, Type&& value)
    {
        Insert(cbegin() + index, std::move(value));
    }
    /// <summary>
    /// 在指定索引位置插入 count 个相同元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="count">要插入的元素数量</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(int32 index, int32 count, const Type& value)
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
    constexpr void Insert(int32 index, InputIt first, InputIt last)
    {
        Insert(cbegin() + index, first, last);
    }
    /// <summary>
    /// 在指定索引位置插入初始化列表中的元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="ilist">初始化列表</param>
    constexpr void Insert(int32 index, std::initializer_list<Type> ilist)
    {
        Insert(cbegin() + index, ilist);
    }
    /// <summary>
    /// 在数组末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(const Type& value)
    {
        Add(value);
    }
    /// <summary>
    /// 在数组末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(Type&& value)
    {
        Add(std::move(value));
    }
    /// <summary>
    /// 移除数组末尾的元素
    /// </summary>
    constexpr void Pop()
    {
        if (m_size == 0) return;
        std::destroy_at(&m_data[m_size - 1]);
        --m_size;
    }
    /// <summary>
    /// 在数组开头添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(const Type& value)
    {
        Insert(cbegin(), value);
    }
    /// <summary>
    /// 在数组开头添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(Type&& value)
    {
        Insert(cbegin(), std::move(value));
    }
    /// <summary>
    /// 移除数组开头的元素
    /// </summary>
    constexpr void Shift()
    {
        if (m_size == 0) return;
        Erase(cbegin());
    }
    /// <summary>
    /// 在数组末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        if (m_size + 1 > m_capacity) 
        {
            Reserve(m_size + 1);
        }
        std::construct_at(&m_data[m_size], std::forward<Args>(args)...);
        ++m_size;
        return end() - 1;
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

        m_size -= static_cast<int32>(count);
        return begin() + first;
    }
    /// <summary>
    /// 移除指定索引位置的元素
    /// </summary>
    /// <param name="index">要移除元素的索引</param>
    constexpr void Erase(int32 index)
    {
        Erase(cbegin() + index);
    }
    /// <summary>
    /// 移除从 start 到 end 索引范围内的元素
    /// </summary>
    /// <param name="start">起始索引</param>
    /// <param name="end">结束索引</param>
    constexpr void Erase(int32 start, int32 end)
    {
        Erase(cbegin() + start, cbegin() + end);
    }
    /// <summary>
    /// 检查数组中是否包含指定值
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>如果找到返回 true，否则返回 false</returns>
    constexpr bool Contains(const Type& value)
    {
        for (int32 i = 0; i < m_size; ++i) 
        {
            if (m_data[i] == value) 
            {
                return true;
            }
        }
        return false;
    }
    /// <summary>
    /// 检查数组中是否有元素满足指定条件
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>如果有元素满足条件返回 true，否则返回 false</returns>
    template<class Condition>
    constexpr bool Includes(Condition&& condition)
    {
        for (int32 i = 0; i < m_size; ++i) 
        {
            if (std::forward<Condition>(condition)(m_data[i])) 
            {
                return true;
            }
        }
        return false;
    }
    /// <summary>
    /// 查找第一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向匹配元素的迭代器</returns>
    template<class Condition>
    constexpr iterator Find(Condition&& condition)
    {
        for (iterator iter = begin(); iter != end(); ++iter)
        {
            if (condition(*iter))
            {
                return iter;
            }
        }
        return end();
    }
    /// <summary>
    /// 查找最后一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向匹配元素的迭代器</returns>
    template<class Condition>
    constexpr iterator FindLast(Condition&& condition)
    {
        for (iterator iter = end() - 1; iter >= begin(); --iter)
        {
            if (condition(*iter))
            {
                return iter;
            }
        }
        return end();
    }
    /// <summary>
    /// 查找第一个满足条件的元素的索引
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>找到的元素的索引，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr int32 FindIndex(Condition&& condition)
    {
        for (int32 i = 0; i < m_size; ++i) 
        {
            if (std::forward<Condition>(condition)(m_data[i])) 
            {
                return i;
            }
        }
        return -1;
    }
    /// <summary>
    /// 查找最后一个满足条件的元素的索引
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>找到的元素的索引，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr int32& FindLastIndex(Condition&& condition)
    {
        for (int32 i = m_size - 1; i >= 0; --i) 
        {
            if (std::forward<Condition>(condition)(m_data[i])) 
            {
                return i;
            }
        }
        return -1;
    }
    /// <summary>
    /// 查找指定值的第一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr int32 IndexOf(const Type& value)
    {
        return FindIndex([&](const Type& element) { return element == value; });
    }
    /// <summary>
    /// 查找指定值的最后一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr int32 LastIndexOf(const Type& value)
    {
        return FindLastIndex([&](const Type& element) { return element == value; });
    }
    /// <summary>
    /// 过滤数组，保留满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新数组</returns>
    template<class Condition>
    constexpr Array& With(Condition&& condition)
    {
        int32 new_size = 0;
        for (int32 i = 0; i < m_size; ++i) {
            if (std::forward<Condition>(condition)(m_data[i])) 
            {
                if (new_size != i) 
                {
                    std::construct_at(&m_data[new_size], std::move(m_data[i]));
                    std::destroy_at(&m_data[i]);
                }
                ++new_size;
            }
            else 
            {
                std::destroy_at(&m_data[i]);
            }
        }
        m_size = new_size;
        return *this;
    }
    /// <summary>
    /// 创建包含满足条件元素的新数组
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新数组</returns>
    template<class Condition>
    constexpr Array Filter(Condition&& condition)
    {
        Array result;
        for (int32 i = 0; i < m_size; ++i) 
        {
            if (std::forward<Condition>(condition)(m_data[i])) 
            {
                result.Add(m_data[i]);
            }
        }
        return result;
    }
    /// <summary>
    /// 根据条件对数组进行排序
    /// </summary>
    /// <param name="condition">排序条件或比较函数</param>
    template<class Condition>
    constexpr void Sort(Condition&& condition)
    {
        std::sort(begin(), end(), std::forward<Condition>(condition));
    }
    /// <summary>
    /// 检查数组是否为空
    /// </summary>
    /// <returns>true表示数组为空</returns>
    constexpr bool IsEmpty() const
    {
        return m_size == 0;
    }
    /// <summary>
    /// 检查索引是否有效
    /// </summary>
    /// <param name="index">要检查的索引</param>
    /// <returns>如果索引有效返回 true，否则返回 false</returns>
    constexpr bool IsValidIndex(int32 index) const
    {
        return index >= 0 && index < m_size;
    }
    /// <summary>
    /// 交换两个数组的内容
    /// </summary>
    /// <param name="other">要交换的另一个数组</param>
    constexpr void Swap(Array& other)
    {
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_alloc, other.m_alloc);
        std::swap(m_data, other.m_data);
    }
public:
    constexpr Type& operator[](int64 index)
    {
        return m_data[index];
    }

    constexpr const Type& operator[](int64 index) const
    {
        return m_data[index];
    }

    template<class Type>
    constexpr friend Array operator+(const Array<Type>& left, const Array<Type>& right)
    {
        Array<Type> result;
        result.Reserve(left.Size() + right.Size());
        result.Append(left);
        result.Append(right);
        return result;
    }

    constexpr Array& operator+=(const Array& other)
    {
        return Append(other);
    }

    template<Concept::EqualComparableType Type>
    constexpr bool operator==(const Array<Type>& other)
    {
        if (m_size != other.m_size) 
        {
            return false;
        }
            
        for (int32 i = 0; i < m_size; ++i) 
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }             
        }
        return true;
    }

    template<Concept::EqualComparableType Type>
    constexpr bool operator!=(const Array<Type>& other)
    {
        if (m_size != other.m_size)
        {
            return true;
        }

        for (int32 i = 0; i < m_size; ++i)
        {
            if (m_data[i] != other.m_data[i])
            {
                return true;
            }
        }
        return false;
    }

    template<Concept::SortComparableType Type>
    constexpr bool operator>(const Array<Type>& other)
    {
        return other < *this;
    }

    template<Concept::SortComparableType Type>
    constexpr bool operator>=(const Array<Type>& other)
    {
        return !(*this < other);
    }

    template<Concept::SortComparableType Type>
    constexpr bool operator<(const Array<Type>& other)
    {
        const int32 min_size = std::min(m_size, other.m_size);

        for (int32 i = 0; i < min_size; ++i) {
            if (m_data[i] < other.m_data[i]) {
                return true;
            }
            if (other.m_data[i] < m_data[i]) {
                return false;
            }
        }

        // 所有比较的元素都相等，比较长度
        return m_size < other.m_size;
    }

    template<Concept::SortComparableType Type>
    constexpr bool operator<=(const Array<Type>& other)
    {
        return !(other < *this);
    }
public:
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return iterator(m_data);
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return const_iterator(m_data);
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return iterator(m_data + m_size);
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return const_iterator(m_data + m_size);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return const_iterator(m_data);
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return const_iterator(m_data + m_size);
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
private:
    int32 m_size = 0;
    int32 m_capacity = 0;
    Allocator m_alloc = Allocator();
    Type* m_data = nullptr;
};