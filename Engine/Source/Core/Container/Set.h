#pragma once

#include "Core.h"

#include <set>

template<class KeyType>
class Set
{
public:
    using key_type = KeyType;
    using value_type = KeyType;
    using size_type = int64;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename std::set<KeyType>::iterator;
    using const_iterator = typename std::set<KeyType>::const_iterator;
    using reverse_iterator = typename std::set<KeyType>::reverse_iterator;
    using const_reverse_iterator = typename std::set<KeyType>::const_reverse_iterator;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Set() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Set() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    Set(const Set& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    Set& operator=(const Set& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    Set(Set&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    Set& operator=(Set&& other) = default;
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template<class InputIt>
    constexpr Set(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr Set(std::initializer_list<value_type> ilist)
        : m_data(ilist)
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Set& operator=(std::initializer_list<value_type> ilist)
    {
        m_data = ilist;
        return *this;
    }
public:
    /// <summary>
    /// 获取容器当前元素数量
    /// </summary>
    /// <returns></returns>
    constexpr size_type Size() const
    {
        return m_data.size();
    }
    /// <summary>
    /// 获取容器能够容纳的最大元素数量
    /// </summary>
    /// <returns></returns>
    constexpr size_type MaxSize() const
    {
        return std::numeric_limits<size_type>::max();
    }
    /// <summary>
    /// 清空容器
    /// </summary>
    constexpr void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(const KeyType& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(KeyType&& value)
    {
        return m_data.insert(std::move(value)).first;
    }
    /// <summary>
    /// 在容器末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        return m_data.emplace(std::forward<Args>(args)...).first;
    }
    /// <summary>
    /// 合并另一个容器中的所有元素到当前容器
    /// </summary>
    /// <param name="other">要合并的容器</param>
    constexpr void Merge(const Set& other)
    {
        m_data.merge(other.m_data);
    }
    /// <summary>
    /// 合并另一个容器中的所有元素到当前容器（移动语义）
    /// </summary>
    /// <param name="other">要合并的容器</param>
    constexpr void Merge(Set&& other)
    {
        m_data.merge(std::move(other.m_data));
    }
    /// <summary>
    /// 插入一个键值对到容器（拷贝语义）
    /// </summary>
    /// <param name="value">要插入的键值对</param>
    /// <returns>指向插入元素的迭代器</returns>
    constexpr iterator Insert(const value_type& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 插入一个键值对到容器（移动语义）
    /// </summary>
    /// <param name="value">要插入的键值对</param>
    /// <returns>指向插入元素的迭代器</returns>
    constexpr iterator Insert(value_type&& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 在指定位置提示附近插入一个键值对（拷贝语义）
    /// </summary>
    /// <param name="iter">插入位置提示</param>
    /// <param name="value">要插入的键值对</param>
    /// <returns>指向插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, const value_type& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 在指定位置提示附近插入一个键值对（移动语义）
    /// </summary>
    /// <param name="iter">插入位置提示</param>
    /// <param name="value">要插入的键值对</param>
    /// <returns>指向插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, value_type&& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 插入迭代器范围内的所有键值对
    /// </summary>
    /// <typeparam name="InputIt">迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template<class InputIt>
    constexpr void Insert(InputIt first, InputIt last)
    {
        m_data.insert(first, last);
    }
    /// <summary>
    /// 插入初始化列表中的所有键值对
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr void Insert(std::initializer_list<value_type> ilist)
    {
        m_data.insert(ilist);
    }
    /// <summary>
    /// 移除指定键的元素
    /// </summary>
    /// <param name="key">要移除的键</param>
    /// <returns>被移除的元素数量（0或1）</returns>
    constexpr size_type Erase(const KeyType& key)
    {
        return m_data.erase(key);
    }
    /// <summary>
    /// 移除指定迭代器位置的元素
    /// </summary>
    /// <param name="iter">指向要移除元素的迭代器</param>
    /// <returns>指向被移除元素之后元素的迭代器</returns>
    constexpr iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }
    /// <summary>
    /// 移除迭代器范围内的所有元素
    /// </summary>
    /// <param name="firstIter">起始迭代器</param>
    /// <param name="lastIter">结束迭代器</param>
    /// <returns>指向最后一个被移除元素之后元素的迭代器</returns>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }
    /// <summary>
    /// 检查容器是否包含指定的键
    /// </summary>
    /// <param name="key">要检查的键</param>
    /// <returns>如果键存在返回true，否则返回false</returns>
    constexpr bool Contains(const KeyType& key) const
    {
        return m_data.contains(key);
    }
    /// <summary>
    /// 查找指定键的元素
    /// </summary>
    /// <param name="key">要查找的键</param>
    /// <returns>指向找到元素的迭代器，如果未找到则返回end()</returns>
    constexpr iterator Find(const KeyType& key)
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 查找指定键的元素（const版本）
    /// </summary>
    /// <param name="key">要查找的键</param>
    /// <returns>指向找到元素的const迭代器，如果未找到则返回cend()</returns>
    constexpr const_iterator Find(const KeyType& key) const
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 检查容器是否为空
    /// </summary>
    /// <returns>如果容器为空返回true，否则返回false</returns>
    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 检查当前容器是否是另一个容器的子集
    /// </summary>
    /// <param name="other">比较的容器</param>
    /// <returns>如果是子集返回true</returns>
    constexpr bool IsSubset(const Set& other) const
    {
        return std::includes(other.m_data.begin(), other.m_data.end(),
            m_data.begin(), m_data.end());
    }
    /// <summary>
    /// 检查当前容器是否是另一个容器的超集
    /// </summary>
    /// <param name="other">比较的容器</param>
    /// <returns>如果是超集返回true</returns>
    constexpr bool IsSuperset(const Set& other) const
    {
        return other.IsSubset(*this);
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    constexpr void Swap(Set& other) noexcept
    {
        m_data.swap(other.m_data);
    }
public:
    /// <summary>
    /// 并集运算符
    /// </summary>
    constexpr friend Set operator|(const Set& left, const Set& right)
    {
        Set result(left);
        result |= right;
        return result;
    }
    /// <summary>
    /// 并集赋值运算符
    /// </summary>
    constexpr Set& operator|=(const Set& other)
    {
        m_data.insert(other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 交集运算符
    /// </summary>
    constexpr friend Set operator&(const Set& left, const Set& right)
    {
        Set result;
        std::set_intersection(
            left.m_data.begin(), left.m_data.end(),
            right.m_data.begin(), right.m_data.end(),
            std::inserter(result.m_data, result.m_data.begin())
        );
        return result;
    }
    /// <summary>
    /// 交集赋值运算符
    /// </summary>
    constexpr Set& operator&=(const Set& other)
    {
        Set result;
        std::set_intersection(
            m_data.begin(), m_data.end(),
            other.m_data.begin(), other.m_data.end(),
            std::inserter(result.m_data, result.m_data.begin())
        );
        m_data.swap(result.m_data);
        return *this;
    }
    /// <summary>
    /// 差集运算符
    /// </summary>
    constexpr friend Set operator^(const Set& left, const Set& right)
    {
        Set result;
        std::set_difference(
            left.m_data.begin(), left.m_data.end(),
            right.m_data.begin(), right.m_data.end(),
            std::inserter(result.m_data, result.m_data.begin())
        );
        return result;
    }
    /// <summary>
    /// 差集赋值运算符
    /// </summary>
    constexpr Set& operator^=(const Set& other)
    {
        Set result;
        std::set_difference(
            m_data.begin(), m_data.end(),
            other.m_data.begin(), other.m_data.end(),
            std::inserter(result.m_data, result.m_data.begin())
        );
        m_data.swap(result.m_data);
        return *this;
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const Set& left, const Set& right)
    {
        return left.m_data == right.m_data;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const Set& left, const Set& right)
    {
        return left.m_data != right.m_data;
    }
    /// <summary>
    /// 小于运算符
    /// </summary>
    constexpr friend bool operator<(const Set& left, const Set& right)
    {
        return left.m_data < right.m_data;
    }
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    constexpr friend bool operator<=(const Set& left, const Set& right)
    {
        return left.m_data <= right.m_data;
    }
    /// <summary>
    /// 大于运算符
    /// </summary>
    constexpr friend bool operator>(const Set& left, const Set& right)
    {
        return left.m_data > right.m_data;
    }
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    constexpr friend bool operator>=(const Set& left, const Set& right)
    {
        return left.m_data >= right.m_data;
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
    /// <summary>
    /// 返回指向最后一个元素的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return m_data.rbegin();
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return m_data.rbegin();
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return m_data.crbegin();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return m_data.rend();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return m_data.rend();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return m_data.crend();
    }
private:
    std::set<KeyType> m_data;
};