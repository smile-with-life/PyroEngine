#pragma once

#include "Core.h"

#include <unordered_map>

template<class KeyType, class ValueType>
class HashMap
{
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const KeyType, ValueType>;
    using size_type = int64;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename std::unordered_map<KeyType, ValueType>::iterator;
    using const_iterator = typename std::unordered_map<KeyType, ValueType>::const_iterator;
    using local_iterator = typename std::unordered_map<KeyType, ValueType>::local_iterator;
    using const_local_iterator = typename std::unordered_map<KeyType, ValueType>::const_local_iterator;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    HashMap() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~HashMap() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    HashMap(const HashMap& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    HashMap& operator=(const HashMap& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    HashMap(HashMap&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    HashMap& operator=(HashMap&& other) = default;
    /// <summary>
    /// 指定初始桶数量的构造函数
    /// </summary>
    constexpr explicit HashMap(size_type count)
        : m_data(count)
    {

    }
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    template<class InputIt>
    constexpr HashMap(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    constexpr HashMap(std::initializer_list<value_type> ilist)
        : m_data(ilist)
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    constexpr HashMap& operator=(std::initializer_list<value_type> ilist)
    {
        m_data = ilist;
        return *this;
    }
public:
    /// <summary>
    /// 访问指定键对应的值
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr ValueType& At(const KeyType& key)
    {
        return m_data.at(key);
    }
    /// <summary>
    /// 访问指定键对应的值（const版本）
    /// </summary>
    constexpr const ValueType& At(const KeyType& key) const
    {
        return m_data.at(key);
    }
    /// <summary>
    /// 获取容器当前元素数量
    /// </summary>
    constexpr size_type Size() const
    {
        return m_data.size();
    }
    /// <summary>
    /// 获取容器能够容纳的最大元素数量
    /// </summary>
    constexpr size_type MaxSize() const
    {
        return m_data.max_size();
    }
    /// <summary>
    /// 预留空间
    /// </summary>
    constexpr void Reserve(size_type count)
    {
        m_data.reserve(count);
    }
    /// <summary>
    /// 清空容器
    /// </summary>
    constexpr void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 添加元素（拷贝语义）
    /// </summary>
    constexpr iterator Add(const value_type& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 添加元素（移动语义）
    /// </summary>
    constexpr iterator Add(value_type&& value)
    {
        return m_data.insert(std::move(value)).first;
    }
    /// <summary>
    /// 就地构造元素
    /// </summary>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        return m_data.emplace(std::forward<Args>(args)...).first;
    }
    /// <summary>
    /// 合并另一个容器（拷贝语义）
    /// </summary>
    constexpr void Merge(const HashMap& other)
    {
        m_data.merge(other.m_data);
    }
    /// <summary>
    /// 合并另一个容器（移动语义）
    /// </summary>
    constexpr void Merge(HashMap&& other) noexcept
    {
        m_data.merge(std::move(other.m_data));
    }
    /// <summary>
    /// 插入元素（拷贝语义）
    /// </summary>
    constexpr iterator Insert(const value_type& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 插入元素（移动语义）
    /// </summary>
    constexpr iterator Insert(value_type&& value)
    {
        return m_data.insert(std::move(value)).first;
    }
    /// <summary>
    /// 插入元素
    /// </summary>
    constexpr iterator Insert(const_iterator iter, const value_type& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 插入元素（移动语义）
    /// </summary>
    constexpr iterator Insert(const_iterator iter, value_type&& value)
    {
        return m_data.insert(iter, std::move(value));
    }
    /// <summary>
    /// 插入迭代器范围内的所有元素
    /// </summary>
    template<class InputIt>
    constexpr void Insert(InputIt first, InputIt last)
    {
        m_data.insert(first, last);
    }
    /// <summary>
    /// 插入初始化列表中的所有元素
    /// </summary>
    constexpr void Insert(std::initializer_list<value_type> ilist)
    {
        m_data.insert(ilist);
    }
    /// <summary>
    /// 移除指定键的元素
    /// </summary>
    constexpr size_type Erase(const KeyType& key)
    {
        return m_data.erase(key);
    }
    /// <summary>
    /// 移除指定迭代器位置的元素
    /// </summary>
    constexpr iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }
    /// <summary>
    /// 移除迭代器范围内的所有元素
    /// </summary>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }
    /// <summary>
    /// 检查是否包含指定的键
    /// </summary>
    constexpr bool Contains(const KeyType& key) const
    {
        return m_data.contains(key);
    }
    /// <summary>
    /// 查找指定键的元素
    /// </summary>
    constexpr iterator Find(const KeyType& key)
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 查找指定键的元素（const版本）
    /// </summary>
    constexpr const_iterator Find(const KeyType& key) const
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 获取当前桶数量
    /// </summary>
    constexpr size_type BucketCount() const
    {
        return m_data.bucket_count();
    }
    /// <summary>
    /// 获取最大桶数量
    /// </summary>
    constexpr size_type MaxBucketCount() const
    {
        return m_data.max_bucket_count();
    }
    /// <summary>
    /// 获取指定键所在的桶索引
    /// </summary>
    constexpr size_type Bucket(const KeyType& key) const
    {
        return m_data.bucket(key);
    }
    /// <summary>
    /// 获取指定桶的大小
    /// </summary>
    constexpr size_type BucketSize(size_type bucketIndex) const
    {
        return m_data.bucket_size(bucketIndex);
    }
    /// <summary>
    /// 获取指定桶的开始迭代器
    /// </summary>
    constexpr local_iterator BucketBegin(size_type bucketIndex)
    {
        return m_data.begin(bucketIndex);
    } 
    /// <summary>
    /// 获取指定桶的开始迭代器（const版本）
    /// </summary>
    constexpr const_local_iterator BucketBegin(size_type bucketIndex) const
    {
        return m_data.begin(bucketIndex);
    }
    /// <summary>
    /// 获取指定桶的结束迭代器
    /// </summary>
    constexpr local_iterator BucketEnd(size_type bucketIndex)
    {
        return m_data.end(bucketIndex);
    }
    /// <summary>
    /// 获取指定桶的结束迭代器（const版本）
    /// </summary>
    constexpr const_local_iterator BucketEnd(size_type bucketIndex) const
    {
        return m_data.end(bucketIndex);
    }
    /// <summary>
    /// 获取哈希函数
    /// </summary>
    constexpr auto HashFunction() const
    {
        return m_data.hash_function();
    }
    /// <summary>
    /// 获取负载因子
    /// </summary>
    constexpr float Factor() const
    {
        return m_data.load_factor();
    }
    /// <summary>
    /// 获取最大负载因子
    /// </summary>
    constexpr float GetMaxFactor() const
    {
        return m_data.max_load_factor();
    }
    /// <summary>
    /// 设置最大负载因子
    /// </summary>
    constexpr void SetMaxFactor(float ml)
    {
        m_data.max_load_factor(ml);
    }
    /// <summary>
    /// 重新哈希
    /// </summary>
    constexpr void Rehash(size_type count)
    {
        m_data.rehash(count);
    }
    /// <summary>
    /// 检查容器是否为空
    /// </summary>
    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    constexpr void Swap(HashMap& other) noexcept
    {
        m_data.swap(other.m_data);
    }
public:
    /// <summary>
    /// 下标运算符，如果键不存在则创建
    /// </summary>
    constexpr ValueType& operator[](const KeyType& key)
    {
        return m_data[key];
    }
    /// <summary>
    /// 下标运算符，如果键不存在则创建（移动语义）
    /// </summary>
    constexpr ValueType& operator[](KeyType&& key)
    {
        return m_data[std::move(key)];
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const HashMap& left, const HashMap& right)
    {
        return left.m_data == right.m_data;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const HashMap& left, const HashMap& right)
    {
        return left.m_data != right.m_data;
    }

public:
    /// <summary>
    /// 返回指向第一个元素的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return m_data.begin();
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }
    /// <summary>
    /// 返回指向末尾的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_data.end();
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_data.end();
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return m_data.cend();
    }
private:
    std::unordered_map<KeyType, ValueType> m_data;
};