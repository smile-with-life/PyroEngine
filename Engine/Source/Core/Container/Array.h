#pragma once
#include "Core/Core.h"
#include "Iterator/Iterator.h"
#include "Allocator/Allocator.h"


template <class Type>
class ArrayIterator
{


};

template<class Type>
class Array 
{
public:
    using iterator = ArrayIterator<Type>;
    using const_iterator = ArrayIterator<const Type>;
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
    /// 检查数组是否为空
    /// </summary>
    /// <returns>true表示数组为空</returns>
    constexpr bool IsEmpty() const;
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
    /// 在末尾附加元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Append(const Type& value);
    /// <summary>
    /// 在末尾附加元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的元素</param>
    /// <returns>this</returns>
    constexpr Array& Append(Type&& value) noexcept;
    /// <summary>
    /// 在末尾附加另一个数组的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(const Array& other);
    /// <summary>
    /// 在末尾附加另一个数组的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的数组</param>
    /// <returns>this</returns>
    constexpr Array& Append(Array&& other) noexcept;

    constexpr iterator Insert(const_iterator iter, const Type& value);

    constexpr iterator Insert(const_iterator iter, Type&& value);

    constexpr iterator Insert(const_iterator iter, int32 count, const Type& value);

    template<class InputIt>
    constexpr iterator Insert(const_iterator iter, InputIt first, InputIt last);

    constexpr iterator Insert(const_iterator iter, std::initializer_list<Type> ilist);

    template<class... Args>
    constexpr iterator Emplace(const_iterator iter, Args&&... args);

    constexpr iterator Erase(const_iterator iter);

    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter);

    constexpr void PushBack(const Type& value);

    constexpr void PushBack(Type&& value);

    template<class... Args>
    constexpr Type& EmplaceBack(Args&&... args);

    constexpr void PopBack();

    constexpr void Swap(Array& other);

    constexpr bool Contains(const Type& value);

    constexpr bool Find();

    constexpr bool FindLast();

    constexpr bool FindIndex();

    constexpr bool FindLastIndex();

    constexpr void ForEach();

    constexpr int32 IndexOf();

    constexpr int32 LastIndexOf();

    constexpr void Sort();

    constexpr void Fill(int32 a, int32 b);

public:

private:
    int32 m_size = 0;
    int32 m_capacity = 0;
    Allocator m_alloc = Allocator();
    Type* m_data = nullptr;
};
