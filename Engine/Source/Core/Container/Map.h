#pragma once
#include "Core.h"
#include <map>

template<class KeyType, class ValueType>
class Map
{
public:
    using key_Type = KeyType;
    using mapped_Type = ValueType;
    using value_type = std::pair<KeyType, ValueType>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = std::map<KeyType, ValueType>::iterator;
    using const_iterator = std::map<KeyType, ValueType>::const_iterator;
    using reverse_iterator = std::map<KeyType, ValueType>::reverse_iterator;
    using const_reverse_iterator = std::map<KeyType, ValueType>::const_reverse_iterator;
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
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr ValueType& At(const KeyType& key)
    {
        return m_data.at(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr const ValueType& At(const KeyType& key) const
    {
        return m_data.at(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    constexpr int32 Size() const
    {
        return m_data.size();
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    constexpr int32 Max() const
    {
        return std::numeric_limits<int32>::max();
    }
    /// <summary>
    /// 
    /// </summary>
    constexpr void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    constexpr iterator Insert(const value_type& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    constexpr iterator Insert(value_type&& value)
    {
        return m_data.insert(value).first;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="iter"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    constexpr iterator Insert(const_iterator iter, const value_type& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="iter"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    constexpr iterator Insert(const_iterator iter, value_type&& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="InputIt"></typeparam>
    /// <param name="first"></param>
    /// <param name="last"></param>
    template<class InputIt>
    constexpr void Insert(InputIt first, InputIt last)
    {
        return m_data.insert(first, last);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ilist"></param>
    constexpr void Insert(std::initializer_list<value_type> ilist)
    {
        return m_data.insert(ilist);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="...Args"></typeparam>
    /// <param name="...args"></param>
    /// <returns></returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        return m_data.try_emplace(std::forward<Args>(args)...).first;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr int32 Erase(const KeyType& key)
    {
        return m_data.erase(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="iter"></param>
    /// <returns></returns>
    constexpr iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="firstIter"></param>
    /// <param name="lastIter"></param>
    /// <returns></returns>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr bool Contains(const KeyType& key) const
    {
        return m_data.contains(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr iterator Find(const KeyType& key)
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr const_iterator Find(const KeyType& key)
    {
        return m_data.find(key);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    constexpr void Merge(Map& other)
    {
        m_data.merge(other.m_data);
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    constexpr void Merge(Map&& other)
    {
        m_data.merge(std::forward<Map>(other.m_data));
    }
    /// <summary>
    /// 过滤容器，保留满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新容器</returns>
    template<class Condition>
    constexpr Map& With(Condition&& condition)
    {
        
    }
    /// <summary>
    /// 创建包含满足条件元素的新容器
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新容器</returns>
    template<class Condition>
    constexpr Map Filter(Condition&& condition)
    {
        
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 检查键值是否有效
    /// </summary>
    /// <param name="key">要检查的键值</param>
    /// <returns>如果键值有效返回 true，否则返回 false</returns>
    constexpr bool IsValidIndex(const KeyType& key) const
    {
        
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    constexpr void Swap(Map& other) noexcept
    {
        m_data.swap(other.m_data);
    }
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr ValueType& operator[](const KeyType& key)
    {
        return m_data[key];
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    constexpr ValueType& operator[](KeyType&& key)
    {
        return m_data[key];
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr friend bool operator==(const Map& other)
    {
        return m_data == other.m_data;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr friend bool operator!=(const Map& other)
    {
        return m_data != other.m_data;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr friend bool operator<(const Map& other)
    {
        return m_data < other.m_data;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr friend bool operator<=(const Map& other)
    {
        return m_data <= other.m_data;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr bool operator>(const Map& other)
    {
        return m_data > other.m_data;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    constexpr friend bool operator>=(const Map& other)
    {
        return m_data >= other.m_data;
    }
    
public:
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return m_data.begin();
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_data.end();
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_data.end();
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return m_data.cend();
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return m_data.rbegin();
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return m_data.rbegin();
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return m_data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return m_data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return m_data.crbegin();
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return m_data.crend();
    }
private:
    std::map<KeyType, ValueType> m_data;
};