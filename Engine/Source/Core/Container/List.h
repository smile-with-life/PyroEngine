#pragma once
#include "Core.h"
#include "Concept/Concept.h"

#include <list>
#include <algorithm>

template<class Type>
class List
{
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = std::list<Type>::iterator;
    using const_iterator = std::list<Type>::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    List() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~List() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    List(const List& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    List& operator=(const List& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    List(List&& other) = default;  
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    List& operator=(List&& other) = default;
    /// <summary>
    /// 构造函数，创建指定大小的容器
    /// </summary>
    /// <param name="count">容器初始大小</param>
    explicit List(int32 count)
        : m_data(count)
    {
        
    }
    /// <summary>
    /// 构造函数，指定容器大小并赋初值
    /// </summary>
    /// <param name="count">容器初始大小</param>
    /// <param name="value">用于填充的初始值</param>
    List(int32 count, const Type& value)
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
    List(InputIt first, InputIt last)
        : m_data(first, last)
    {
        
    }
    /// <summary>
    /// 初始化列表构造函数
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    List(std::initializer_list<Type> ilist)
        : m_data(ilist)
    {

    }
    /// <summary>
    /// 初始化列表赋值运算符
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    List& operator=(std::initializer_list<Type> ilist)
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
    Type& Front()
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问第一个元素(const版本)
    /// </summary>
    /// <returns>返回首元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    const Type& Front() const
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问最后一个元素
    /// </summary>
    /// <returns>返回尾元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    Type& Back()
    {
        return m_data.back();
    }
    /// <summary>
    /// 访问最后一个元素(const版本)
    /// </summary>
    /// <returns>返回尾元素的const引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    const Type& Back() const
    {
        return m_data.back();
    }
    /// <summary>
    /// 获取容器当前元素数量
    /// </summary>
    /// <returns>元素数量</returns>
    int32 Size() const
    {
        return m_data.size();
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
    /// 清空容器(元素被销毁，容量保持不变)
    /// </summary>
    void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 调整容器大小
    /// </summary>
    /// <param name="size">新的容器大小</param>
    void Resize(int32 size)
    {
        m_data.resize(size);
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(拷贝语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    List& Append(const List& other)
    {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个容器的所有元素(移动语义)
    /// </summary>
    /// <param name="other">要添加的容器</param>
    /// <returns>this</returns>
    List& Append(List&& other) noexcept
    {
        m_data.splice(m_data.end(), std::move(other.m_data));
        return *this;
    }
    /// <summary>
    /// 在容器末尾追加从 first 到 last 范围内的元素
    /// </summary>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    /// <returns>this</returns>
    template <class InputIt>
    List& Append(InputIt first, InputIt last)
    {
        m_data.insert(m_data.end(), first, last);
        return *this;
    }
    /// <summary>
    /// 在容器末尾追加初始化列表中的元素
    /// </summary>
    /// <param name="ilist">初始化列表</param>
    /// <returns>this</returns>
    List& Append(std::initializer_list<Type> ilist)
    {
        m_data.insert(m_data.end(), ilist.begin(), ilist.end());
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    List& Prepend(const List& other)
    {
        m_data.insert(m_data.begin(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    List& Prepend(List&& other)
    {
        m_data.splice(m_data.begin(), std::move(other.m_data));
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="InputIt"></typeparam>
    /// <param name="first"></param>
    /// <param name="last"></param>
    /// <returns></returns>
    template <class InputIt>
    List& Prepend(InputIt first, InputIt last)
    {
        m_data.insert(m_data.begin(), first, last);
        return *this;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="ilist"></param>
    /// <returns></returns>
    List& Prepend(std::initializer_list<Type> ilist)
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
    iterator Insert(const_iterator iter, const Type& value)
    {
        return m_data.insert(iter, value);
    }
    /// <summary>
    /// 在指定位置插入一个元素(移动语义)
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="value">要插入的值</param>
    /// <returns>指向新插入元素的迭代器</returns>
    iterator Insert(const_iterator iter, Type&& value)
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
    iterator Insert(const_iterator iter, int32 count, const Type& value)
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
    iterator Insert(const_iterator iter, InputIt first, InputIt last)
    {
        return m_data.insert(iter, first, last);
    }
    /// <summary>
    /// 在指定位置插入初始化列表中的元素
    /// </summary>
    /// <param name="iter">插入位置的迭代器</param>
    /// <param name="ilist">初始化列表</param>
    /// <returns>指向第一个新插入元素的迭代器</returns>
    iterator Insert(const_iterator iter, std::initializer_list<Type> ilist)
    {
        return m_data.insert(iter, ilist.begin(), ilist.end());
    }

    /// <summary>
    /// 在容器末尾添加一个元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    void Push(const Type& value)
    {
        m_data.push_back(value);
    }
    
    /// <summary>
    /// 在容器末尾添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    void Push(Type&& value)
    {
        m_data.push_back(std::move(value));
    }
    
    /// <summary>
    /// 移除容器末尾的元素
    /// </summary>
    void Pop()
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
    void Unshift(const Type& value)
    {
        m_data.push_front(value);
    }
    /// <summary>
    /// 在容器开头添加一个元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    void Unshift(Type&& value)
    {
        m_data.push_front(std::move(value));
    }
    /// <summary>
    /// 移除容器开头的元素
    /// </summary>
    void Shift()
    {
        if (!IsEmpty())
        {
            m_data.pop_front();
        }   
    }
    /// <summary>
    /// 在容器末尾就地构造一个元素
    /// </summary>
    /// <param name="args">构造元素的参数</param>
    /// <returns>指向新构造元素的迭代器</returns>
    template<class... Args>
    iterator Emplace(const_iterator pos, Args&&... args)
    {
        return m_data.emplace(pos, std::forward<Args>(args)...);
    }
    /// <summary>
    /// 移除指定位置的元素
    /// </summary>
    /// <param name="iter">要移除元素的迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }
    /// <summary>
    /// 移除从 firstIter 到 lastIter 范围内的元素
    /// </summary>
    /// <param name="firstIter">起始迭代器</param>
    /// <param name="lastIter">结束迭代器</param>
    /// <returns>指向被移除元素后面元素的迭代器</returns>
    iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }
    /// <summary>
    /// 检查容器中是否包含指定值
    /// </summary>
    /// <param name="value">要查找的值</param>
    /// <returns>如果找到返回 true，否则返回 false</returns>
    bool Contains(const Type& value)
    {
        return std::find(m_data.begin(), m_data.end(), value) != m_data.end();
    }
    /// <summary>
    /// 检查容器中是否有元素满足指定条件
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>如果有元素满足条件返回 true，否则返回 false</returns>
    template<class Condition>
    bool Includes(Condition&& condition)
    {
        return std::find_if(m_data.begin(), m_data.end(), std::forward<Condition>(condition)) != m_data.end();
    }
    /// <summary>
    /// 查找第一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向匹配元素的迭代器</returns>
    template<class Condition>
    iterator Find(Condition&& condition)
    {
        return std::find_if(m_data.begin(), m_data.end(), std::forward<Condition>(condition));
    
    }
    /// <summary>
    /// 查找最后一个满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>指向匹配元素的迭代器</returns>
    template<class Condition>
    iterator FindLast(Condition&& condition)
    {
        return std::find_if(m_data.rbegin(), m_data.rend(), std::forward<Condition>(condition)).base();
    }
    /// <summary>
    /// 归并两个已排序容器
    /// </summary>
    /// <param name="other"></param>
    void Merge(List& other)
    {
        m_data.merge(other.m_data);
    }
    /// <summary>
    /// 归并两个已排序容器
    /// </summary>
    /// <param name="other"></param>
    void Merge(List&& other)
    {
        m_data.merge(std::move(other.m_data));
    }
    /// <summary>
    /// 移除满足指定的元素
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    int32 Remove(const Type& value)
    {
        return m_data.remove(value);
    }
    /// <summary>
    /// 过滤容器，保留满足条件的元素
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新容器</returns>
    template<class Condition>
    List& With(Condition&& condition)
    {
        m_data.remove_if([&](const Type& elem) { return !condition(elem); });
        return *this;
    }
    /// <summary>
    /// 创建包含满足条件元素的新容器
    /// </summary>
    /// <param name="condition">条件函数或谓词</param>
    /// <returns>包含满足条件元素的新容器</returns>
    template<class Condition>
    List Filter(Condition&& condition)
    {
        List<Type> result;
        for (const auto& elem : m_data)
        {
            if (condition(elem))
            {
                result.Push(elem);
            }
        }
        return result;
    }
    /// <summary>
    /// 反转元素的顺序
    /// </summary>
    void Reverse()
    {
        m_data.reverse();
    }
    /// <summary>
    /// 根据条件对容器进行排序
    /// </summary>
    /// <param name="condition">排序条件或比较函数</param>
    template<class Condition>
    void Sort(Condition&& condition)
    {
        m_data.sort(std::forward<Condition>(condition));
    }
    /// <summary>
    /// 检查容器是否为空
    /// </summary>
    /// <returns>true表示容器为空</returns>
    bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    void Swap(List& other)
    {
        std::swap(m_data, other.m_data);
    }
public:
    template<class Type>
    friend List operator+(const List<Type>& left, const List<Type>& right)
    {
        List<Type> result;
        result.Reserve(left.Size() + right.Size());
        result.Append(left);
        result.Append(right);
        return result;
    }

    List& operator+=(const List& other)
    {
        return m_data += other.m_data;
    }

    friend bool operator==(const List& other)
    {
        return m_data == other.m_data;
    }

    friend bool operator!=(const List& other)
    {
        return m_data != other.m_data;
    }

    friend bool operator<(const List& other)
    {
        return m_data < other.m_data;
    }

    friend bool operator<=(const List& other)
    {
        return m_data <= other.m_data;
    }

    friend bool operator>(const List& other)
    {
        return m_data > other.m_data;
    }

    friend bool operator>=(const List& other)
    {
        return m_data >= other.m_data;
    }   
public:
    [[nodiscard]] iterator begin() noexcept
    {
        return iterator(m_data);
    }

    [[nodiscard]] const_iterator begin() const noexcept
    {
        return const_iterator(m_data);
    }

    [[nodiscard]] iterator end() noexcept
    {
        return iterator(m_data + m_size);
    }

    [[nodiscard]] const_iterator end() const noexcept
    {
        return const_iterator(m_data + m_size);
    }

    [[nodiscard]] const_iterator cbegin() const noexcept
    {
        return const_iterator(m_data);
    }

    [[nodiscard]] const_iterator cend() const noexcept
    {
        return const_iterator(m_data + m_size);
    }

    [[nodiscard]] reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
private:
    std::list<Type> m_data;
};