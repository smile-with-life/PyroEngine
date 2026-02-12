#pragma once

#include "Core.h"

#include <list>
#include <algorithm>

template<class Type>
class List
{
public:
    using value_type = Type;
    using size_type = int64;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename std::list<value_type>::iterator;
    using const_iterator = typename std::list<value_type>::const_iterator;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr List() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~List() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    constexpr List(const List& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    constexpr List& operator=(const List& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    constexpr List(List&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    constexpr List& operator=(List&& other) = default;
    /// <summary>
    /// 构造函数，创建指定大小的容器
    /// </summary>
    /// <param name="count">容器初始大小</param>
    constexpr explicit List(size_type count)
        : m_data(count)
    {

    }
    /// <summary>
    /// 构造函数，指定容器大小并赋初值
    /// </summary>
    /// <param name="count">容器初始大小</param>
    /// <param name="value">用于填充的初始值</param>
    constexpr List(size_type count, const Type& value)
        : m_data(count, value)
    {

    }
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template <class InputIt>
    constexpr List(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    constexpr List(std::initializer_list<Type> ilist)
        : m_data(ilist)
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr List& operator=(std::initializer_list<Type> ilist)
    {
        m_data = ilist;
        return *this;
    }
public:
    /// <summary>
    /// 访问第一个元素
    /// </summary>
    /// <returns>返回首元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr Type& Front()
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问第一个元素(const版本)
    /// </summary>
    /// <returns>返回首元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr const Type& Front() const
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问最后一个元素
    /// </summary>
    /// <returns>返回尾元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr Type& Back()
    {
        return m_data.back();
    }
    /// <summary>
    /// 访问最后一个元素(const版本)
    /// </summary>
    /// <returns>返回尾元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    constexpr const Type& Back() const
    {
        return m_data.back();
    }
    /// <summary>
    /// 获取容器当前元素数量
    /// </summary>
    /// <returns>元素数量</returns>
    constexpr size_type Size() const
    {
        return m_data.size();
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
    /// 清空容器
    /// </summary>
    constexpr void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 调整容器大小
    /// </summary>
    /// <param name="size">新的容器大小</param>
    constexpr void Resize(size_type size)
    {
        m_data.resize(size);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(const Type& value)
    {
        return m_data.insert(m_data.end(), value);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    /// <returns>指向新元素的迭代器</returns>
    constexpr iterator Add(Type&& value)
    {
        return m_data.insert(m_data.end(), std::move(value));
    }
    /// <summary>
    /// 在容器末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        return m_data.emplace(m_data.end(), std::forward<Args>(args)...);
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
        
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr List& Append(const List& other)
    {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr List& Append(List&& other) noexcept
    {
        m_data.splice(m_data.end(), other.m_data);
        return *this;
    }
    /// <summary>
    /// 在容器末尾追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <class InputIt>
    constexpr List& Append(InputIt first, InputIt last)
    {
        m_data.insert(m_data.end(), first, last);
        return *this;
    }
    /// <summary>
    /// 在容器末尾追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr List& Append(std::initializer_list<Type> ilist)
    {
        m_data.insert(m_data.end(), ilist.begin(), ilist.end());
        return *this;
    }
    /// <summary>
    /// 在起始追加另一个容器的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr List& Prepend(const List& other)
    {
        m_data.insert(m_data.begin(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 在起始追加另一个容器的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    constexpr List& Prepend(List&& other)
    {
        m_data.splice(m_data.begin(), std::move(other.m_data));
        return *this;
    }
    /// <summary>
    /// 在容器起始追加从 first 到 last 范围内的元素
    /// </summary>
    /// <typeparam name="InputIt">起始迭代器</typeparam>
    /// <param name="first"></param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <class InputIt>
    constexpr List& Prepend(InputIt first, InputIt last)
    {
        m_data.insert(m_data.begin(), first, last);
        return *this;
    }
    /// <summary>
    /// 在容器起始追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    constexpr List& Prepend(std::initializer_list<Type> ilist)
    {
        m_data.insert(m_data.begin(), ilist.begin(), ilist.end());
        return *this;
    }
    /// <summary>
    /// 在指定位置插入一个元素(拷贝语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, const Type& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 在指定位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, Type&& value)
    {
        return m_data.insert(iter, std::move(value));
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
        return m_data.insert(iter, count, value);
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
        return m_data.insert(iter, first, last);
    }
    /// <summary>
    /// 在指定位置插入初始化列表中的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="ilist">初始化列表</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    constexpr iterator Insert(const_iterator iter, std::initializer_list<Type> ilist)
    {
        return m_data.insert(iter, ilist.begin(), ilist.end());
    }
    /// <summary>
    /// 移除指定位置的元素
    /// </summary>
    /// <param name="iter">要移除元素的迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }
    /// <summary>
    /// 移除从 firstIter 到 lastIter 范围内的元素
    /// </summary>
    /// <param name="firstIter">起始迭代器</param>
    /// <param name="lastIter">结束迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(const Type& value)
    {
        m_data.push_back(value);
    }
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Push(Type&& value)
    {
        m_data.push_back(std::move(value));
    }
    /// <summary>
    /// 移除容器末尾的元素
    /// </summary>
    constexpr void Pop()
    {
        if (!IsEmpty())
        {
            m_data.pop_back();
        }
    }
    /// <summary>
    /// 在容器开头添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(const Type& value)
    {
        m_data.push_front(value);
    }
    /// <summary>
    /// 在容器开头添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    constexpr void Unshift(Type&& value)
    {
        m_data.push_front(std::move(value));
    }
    /// <summary>
    /// 移除容器开头的元素
    /// </summary>
    constexpr void Shift()
    {
        if (!IsEmpty())
        {
            m_data.pop_front();
        }
    }
    /// <summary>
    /// 检查容器中是否包含指定值
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>如果找到返回 true，否则返回 false</returns>
    constexpr bool Contains(const Type& value)
    {
        return std::find(m_data.begin(), m_data.end(), value) != m_data.end();
    }
    /// <summary>
    /// 查找第一个满足条件的元素
    /// </summary>
    /// <param name="value">指定元素</param>
    /// <returns>指向匹配元素的迭代器</returns>
    constexpr iterator Find(const Type& value)
    {
        return std::find(m_data.begin(), m_data.end(), value);
    }
    /// <summary>
    /// 查找最后一个满足条件的元素
    /// </summary>
    /// <param name="value">指定元素</param>
    /// <returns>指向匹配元素的迭代器</returns>
    template<class Condition>
    constexpr iterator FindLast(const Type& value)
    {
        auto iter = std::find(m_data.rbegin(), m_data.rend(), value);
        return iter == m_data.rend() ? m_data.end() : std::prev(it.base());
    }
    /// <summary>
    /// 反转元素的顺序
    /// </summary>
    constexpr void Reverse()
    {
        m_data.reverse();
    }
    /// <summary>
    /// 移除容器中所有等于指定值的元素
    /// </summary>
    /// <param name="value">要移除的元素值</param>
    constexpr void RemoveAll(const Type& value)
    {
        m_data.remove(value);
    }
    /// <summary>
    /// 检查容器是否为空
    /// </summary>
    /// <returns>true表示容器为空</returns>
    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    constexpr void Swap(List& other)
    {
        std::swap(m_data, other.m_data);
    }
public:
    /// <summary>
    /// 加法运算符
    /// </summary>
    constexpr friend List operator+(const List& left, const List& right)
    {
        List<Type> result(left);
        result.Append(right);
        return result;
    }
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    constexpr List& operator+=(const List& other)
    {
        Append(other);
        return *this;
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const List& left, const List& right)
    {
        return left.m_data == right.m_data;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const List& left, const List& right)
    {
        return left.m_data != right.m_data;
    }
    /// <summary>
    /// 小于运算符
    /// </summary>
    constexpr friend bool operator<(const List& left, const List& right)
    {
        return left.m_data < right.m_data;
    }
    /// <summary>
    /// 小于等于运算符
    /// </summary>
    constexpr friend bool operator<=(const List& left, const List& right)
    {
        return left.m_data <= right.m_data;
    }
    /// <summary>
    /// 大于运算符
    /// </summary>
    constexpr friend bool operator>(const List& left, const List& right)
    {
        return left.m_data > right.m_data;
    }
    /// <summary>
    /// 大于等于运算符
    /// </summary>
    constexpr friend bool operator>=(const List& left, const List& right)
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
    std::list<Type> m_data;
};