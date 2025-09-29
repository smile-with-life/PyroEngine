#pragma once

#include "Core.h"
#include "Concept.h"
#include "Iterator/Iterator.h"
#include "Allocator/Allocator.h"


template <class Type>
class ArrayIterator
{
public:
    using category = ContiguousIteratorTag;
public:
    constexpr ArrayIterator() noexcept
        : m_ptr()
    {
    }

    explicit constexpr ArrayIterator(const Type* ptr) noexcept
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
        ArrayConstIterator tmp = *this;
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
        ArrayConstIterator tmp = *this;
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
    Type* m_ptr;
};

template <class Type>
class ArrayConstIterator
{
public:
    using category = ContiguousIteratorTag;
public:
    constexpr ArrayConstIterator() noexcept
        : m_ptr()
    {
    }

    explicit constexpr ArrayConstIterator(Type* ptr) noexcept
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
    const Type* m_ptr;

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
    constexpr Array();
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~Array();
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的数组</param>
    constexpr Array(const Array& other);
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的数组</param>
    /// <returns>this</returns>
    constexpr Array& operator=(const Array& other);
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的数组</param>
    constexpr Array(Array&& other);
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的数组</param>
    /// <returns>this</returns>
    constexpr Array& operator=(Array&& other);
    /// <summary>
    /// 构造函数，创建指定大小的数组
    /// </summary>
    /// <param name="count">数组初始大小</param>
    constexpr explicit Array(int32 count);
    /// <summary>
    /// 构造函数，指定数组大小并赋初值
    /// </summary>
    /// <param name="count">数组初始大小</param>
    /// <param name="value">用于填充的初始值</param>
    constexpr Array(int32 count, const Type& value);
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template <std::random_access_iterator InputIt>
    constexpr Array(InputIt first, InputIt last);
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr Array(std::initializer_list<Type> ilist);
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& operator=(std::initializer_list<Type> ilist);
public:
    /// <summary>
    /// 访问指定位置的元素
    /// </summary>
    /// <param name="index">位置索引</param>
    /// <returns>返回元素的引用</returns>
    /// <exception cref="std::out_of_range">当index超出范围时抛出</exception>
    constexpr Type& At(int32 index);
    /// <summary>
    /// 访问指定位置的元素(const版本)
    /// </summary>
    /// <param name="index">位置索引</param>
    /// <returns>返回元素的const引用</returns>
    /// <exception cref="std::out_of_range">当index超出范围时抛出</exception>
    constexpr const Type& At(int32 index) const;
    /// <summary>
    /// 访问第一个元素
    /// </summary>
    /// <returns>返回首元素的引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    Type& Front();
    /// <summary>
    /// 访问第一个元素(const版本)
    /// </summary>
    /// <returns>返回首元素的const引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr const Type& Front() const;
    /// <summary>
    /// 访问最后一个元素
    /// </summary>
    /// <returns>返回尾元素的引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr Type& Back();
    /// <summary>
    /// 访问最后一个元素(const版本)
    /// </summary>
    /// <returns>返回尾元素的const引用</returns>
    /// <exception cref="std::out_of_range">数组为空时抛出</exception>
    constexpr const Type& Back() const;
    /// <summary>
    /// 获取底层数组指针
    /// </summary>
    /// <returns>指向数组首元素的指针</returns>
    constexpr Type* Data();
    /// <summary>
    /// 获取底层数组指针(const版本)
    /// </summary>
    /// <returns>指向数组首元素的const指针</returns>
    constexpr const Type* Data() const;
    /// <summary>
    /// 获取数组当前已使用的内存字节大小
    /// </summary>
    /// <returns>已使用的内存字节大小</returns>
    constexpr uint64 ByteSize() const;
    /// <summary>
    /// 获取模板参数类型Type的字节大小
    /// </summary>
    /// <returns>类型Type的字节大小</returns>
    constexpr int32 TypeSize();
    /// <summary>
    /// 获取数组当前元素数量
    /// </summary>
    /// <returns>元素数量</returns>
    constexpr int32 Size() const;
    /// <summary>
    /// 获取容器能够容纳的最大元素数量
    /// </summary>
    /// <returns>最大元素数量</returns>
    int32 Max() const;
    /// <summary>
    /// 获取数组当前容量
    /// </summary>
    /// <returns>当前容量</returns>
    constexpr int32 Capacity() const;
    /// <summary>
    /// 清空数组(元素被销毁，容量保持不变)
    /// </summary>
    constexpr void Clear();
    /// <summary>
    /// 预留存储空间(避免多次重新分配)
    /// </summary>
    /// <param name="size">期望的最小容量</param>
    constexpr void Reserve(int32 size);
    /// <summary>
    /// 调整数组大小
    /// </summary>
    /// <param name="size">新的数组大小</param>
    constexpr void Resize(int32 size);
    /// <summary>
    /// 释放未使用的内存
    /// </summary>
    constexpr void Shrink();
    /// <summary>
    /// 在末尾添加元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Add(const Type& value);
    /// <summary>
    /// 在末尾添加元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Add(Type&& value) noexcept;
    /// <summary>
    /// 在末尾追加另一个数组的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(const Array& other);
    /// <summary>
    /// 在末尾追加另一个数组的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(Array&& other) noexcept;
    /// <summary>
    /// 在数组末尾追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <std::random_access_iterator InputIt>
    constexpr Array& Append(InputIt first, InputIt last);
    /// <summary>
    /// 在数组末尾追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Array& Append(std::initializer_list<Type> ilist);  
    /// <summary>
    /// 在指定位置插入一个元素(拷贝语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, const Type& value);
    /// <summary>
    /// 在指定位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, Type&& value);
    /// <summary>
    /// 在指定位置插入 count 个相同元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="count">要插入的元素数量</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, int32 count, const Type& value);
    /// <summary>
    /// 在指定位置插入从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    template<class InputIt>
    constexpr iterator Insert(const_iterator iter, InputIt first, InputIt last);
    /// <summary>
    /// 在指定位置插入初始化列表中的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="ilist">初始化列表</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, std::initializer_list<Type> ilist);
    /// <summary>
    /// 在指定索引位置插入一个元素(拷贝语义)
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(int32 index, const Type& value);
    /// <summary>
    /// 在指定索引位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(int32 index, Type&& value);
    /// <summary>
    /// 在指定索引位置插入 count 个相同元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="count">要插入的元素数量</param>
    /// <param name="value">要插入的值</param>
    constexpr void Insert(int32 index, int32 count, const Type& value);
    /// <summary>
    /// 在指定索引位置插入从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template<class InputIt>
    constexpr void Insert(int32 index, InputIt first, InputIt last);
    /// <summary>
    /// 在指定索引位置插入初始化列表中的元素
    /// </summary>
    /// <param name="index">插入位置的索引</param>
    /// <param name="ilist">初始化列表</param>
    constexpr void Insert(int32 index, std::initializer_list<Type> ilist);
    /// <summary>
    /// 在数组末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(const Type& value);
    /// <summary>
    /// 在数组末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(Type&& value);
    /// <summary>
    /// 移除数组末尾的元素
    /// </summary>
    constexpr void Pop();
    /// <summary>
    /// 在数组开头添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(const Type& value);
    /// <summary>
    /// 在数组开头添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(Type&& value);
    /// <summary>
    /// 移除数组开头的元素
    /// </summary>
    constexpr void Shift();
    /// <summary>
    /// 在数组末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args);
    /// <summary>
    /// 移除指定位置的元素
    /// </summary>
    /// <param name="iter">要移除元素的迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator iter);
    /// <summary>
    /// 移除从 firstIter 到 lastIter 范围内的元素
    /// </summary>
    /// <param name="firstIter">起始迭代器</param>
    /// <param name="lastIter">结束迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter);
    /// <summary>
    /// 移除指定索引位置的元素
    /// </summary>
    /// <param name="index">要移除元素的索引</param>
    constexpr void Erase(int32 index);
    /// <summary>
    /// 移除从 start 到 end 索引范围内的元素
    /// </summary>
    /// <param name="start">起始索引</param>
    /// <param name="end">结束索引</param>
    constexpr void Erase(int32 start, int32 end);    
    /// <summary>
    /// 检查数组中是否包含指定值
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>如果找到返回 true，否则返回 false</returns>
    constexpr bool Contains(const Type& value);
    /// <summary>
    /// 检查数组中是否有元素满足指定条件
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>如果有元素满足条件返回 true，否则返回 false</returns>
    template<class Condition>
    constexpr bool Includes(Condition&& condition);
    /// <summary>
    /// 查找第一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向找到的元素的引用，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr Type& Find(Condition&& condition);
    /// <summary>
    /// 查找最后一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向找到的元素的引用，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr Type& FindLast(Condition&& condition);
    /// <summary>
    /// 查找第一个满足条件的元素的索引
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>找到的元素的索引，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr int32 FindIndex(Condition&& condition);
    /// <summary>
    /// 查找最后一个满足条件的元素的索引
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>找到的元素的索引，如果未找到则行为未定义</returns>
    template<class Condition>
    constexpr int32& FindLastIndex(Condition&& condition);
    /// <summary>
    /// 查找指定值的第一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr int32 IndexOf(const Type& value);
    /// <summary>
    /// 查找指定值的最后一个出现位置
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>找到的索引，如果未找到返回 -1</returns>
    constexpr int32 LastIndexOf(const Type& value);
    /// <summary>
    /// 过滤数组，保留满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新数组</returns>
    template<class Condition>
    constexpr Array& Filter(Condition&& condition);
    /// <summary>
    /// 创建包含满足条件元素的新数组
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新数组</returns>
    template<class Condition>
    constexpr Array With(Condition&& condition);
    /// <summary>
    /// 根据条件对数组进行排序
    /// </summary>
    /// <param name="condition">排序条件或比较函数</param>
    template<class Condition>
    constexpr void Sort(Condition&& condition);
    /// <summary>
    /// 检查数组是否为空
    /// </summary>
    /// <returns>true表示数组为空</returns>
    constexpr bool IsEmpty() const;
    /// <summary>
    /// 检查索引是否有效
    /// </summary>
    /// <param name="index">要检查的索引</param>
    /// <returns>如果索引有效返回 true，否则返回 false</returns>
    constexpr bool IsValidIndex(int32 index) const;
    /// <summary>
    /// 检查索引和数量是否在有效范围内
    /// </summary>
    /// <param name="index">起始索引</param>
    /// <param name="count">元素数量</param>
    constexpr void RangeCheck(int32 index, int32 count) const;
    /// <summary>
    /// 交换两个数组的内容
    /// </summary>
    /// <param name="other">要交换的另一个数组</param>
    constexpr void Swap(Array& other);
    /// <summary>
    /// @暂定
    /// </summary>
    /// <returns></returns>
    constexpr int32 AllocateSize();
    /// <summary>
    /// @暂定
    /// </summary>
    constexpr void ForEach();
    /// <summary>
    /// @暂定
    /// </summary>
    /// <param name="count"></param>
    /// <param name="value"></param>
    constexpr void Fill(int32 count, const Type& value);
public:
    constexpr Type& operator[](int64 index);

    constexpr const Type& operator[](int64 index) const;

    template<class Type>
    constexpr friend Array operator+(const Array<Type>& left, const Array<Type>& right);

    constexpr Array& operator+=(const Array& other);

    template<Concept::EqualComparableType Type>
    constexpr bool operator==(const Array<Type>& other);

    template<Concept::EqualComparableType Type>
    constexpr bool operator!=(const Array<Type>& other);

    template<Concept::SortComparableType Type>
    constexpr bool operator>(const Array<Type>& other);

    template<Concept::SortComparableType Type>
    constexpr bool operator>=(const Array<Type>& other);

    template<Concept::SortComparableType Type>
    constexpr bool operator<(const Array<Type>& other);

    template<Concept::SortComparableType Type>
    constexpr bool operator<=(const Array<Type>& other);
public:
    [[nodiscard]] constexpr iterator begin() noexcept;

    [[nodiscard]] constexpr const_iterator begin() const noexcept;

    [[nodiscard]] constexpr iterator end() noexcept;

    [[nodiscard]] constexpr const_iterator end() const noexcept;

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept;

    [[nodiscard]] constexpr const_iterator cend() const noexcept;

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept;

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;

    [[nodiscard]] constexpr reverse_iterator rend() noexcept;

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;
private:
    int32 m_size = 0;
    int32 m_capacity = 0;
    Allocator m_alloc = Allocator();
    Type* m_data = nullptr;
};
