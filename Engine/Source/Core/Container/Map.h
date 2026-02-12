#pragma once

#include "Core.h"

#include <map>

template<class KeyType, class ValueType>
class Map
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
    using iterator = typename std::map<KeyType, ValueType>::iterator;
    using const_iterator = typename std::map<KeyType, ValueType>::const_iterator;
    using reverse_iterator = typename std::map<KeyType, ValueType>::reverse_iterator;
    using const_reverse_iterator = typename std::map<KeyType, ValueType>::const_reverse_iterator;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Map() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Map() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    Map(const Map& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    Map& operator=(const Map& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    Map(Map&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    Map& operator=(Map&& other) = default;
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template<class InputIt>
    constexpr Map(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr Map(std::initializer_list<value_type> ilist)
        : m_data(ilist)
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr Map& operator=(std::initializer_list<value_type> ilist)
    {
        m_data = ilist;
        return *this;
    }
public:
    /// <summary>
    /// 访问指定键对应的值
    /// </summary>
    /// <param name="key">要查找的键</param>
    /// <returns>返回对应值的引用</returns>
    constexpr ValueType& At(const KeyType& key)
    {
        return m_data.at(key);
    }
    /// <summary>
    /// 访问指定键对应的值(const版本)
    /// </summary>
    /// <param name="key">要查找的键</param>
    /// <returns>返回对应值的引用</returns>
    constexpr const ValueType& At(const KeyType& key) const
    {
        return m_data.at(key);
    }
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
    constexpr iterator Add(const value_type& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(value_type&& value)
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
    constexpr void Merge(const Map& other)
    {
        m_data.merge(other.m_data);
    }
    /// <summary>
    /// 合并另一个容器中的所有元素到当前容器（移动语义）
    /// </summary>
    /// <param name="other">要合并的容器</param>
    constexpr void Merge(Map&& other)
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
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    constexpr void Swap(Map& other) noexcept
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
        return m_data[key];
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const Map& left, const Map& right)
    {
        return left.m_data == right.m_data;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const Map& left, const Map& right)
    {
        return left.m_data != right.m_data;
    }
    /// <summary>
    /// 小于运算符
    /// </summary>
    constexpr friend bool operator<(const Map& left, const Map& right)
    {
        return left.m_data < right.m_data;
    }
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    constexpr friend bool operator<=(const Map& left, const Map& right)
    {
        return left.m_data <= right.m_data;
    }
    /// <summary>
    /// 大于运算符
    /// </summary>
    constexpr friend bool operator>(const Map& left, const Map& right)
    {
        return left.m_data > right.m_data;
    }
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    constexpr friend bool operator>=(const Map& left, const Map& right)
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
    std::map<KeyType, ValueType> m_data;
};