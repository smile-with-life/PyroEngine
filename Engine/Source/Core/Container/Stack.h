#pragma once
#include "Core.h"
#include <stack>

template<class Type>
class Stack
{
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Stack() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Stack() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    Stack(const Stack& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的容器</param>
    /// <returns>this</returns>
    Stack& operator=(const Stack& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的容器</param>
    Stack(Stack&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的容器</param>
    /// <returns>this</returns>
    Stack& operator=(Stack&& other) = default;
    /// <summary>
    /// 迭代器范围构造函数
    /// </summary>
    /// <typeparam name="InputIt">随机访问迭代器类型</typeparam>
    /// <param name="first">起始迭代器</param>
    /// <param name="last">结束迭代器</param>
    template <class InputIt>
    Stack(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }
public:
    /// <summary>
    /// 访问顶部元素
    /// </summary>
    /// <returns>返回顶部元素的引用</returns>
    /// <exception cref="std::out_of_range">容器为空时抛出</exception>
    Type& Top()
    {
        return m_data.top();
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
        std::stack<Type> emptyStack;
        Swap(emptyStack);
    }
    /// <summary>
    /// 在栈顶插入元素(拷贝语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    void Push(const Type& value)
    {
        m_data.push(value);
    }
    /// <summary>
    /// 在栈顶插入元素(移动语义)
    /// </summary>
    /// <param name="value">要添加的值</param>
    void Push(Type&& value)
    {
        m_data.push(std::move(value));
    }
    /// <summary>
    /// 移除顶部元素
    /// </summary>
    void Pop()
    {
        if (!IsEmpty())
        {
            m_data.pop();
        }
    }
    /// <summary>
    /// 是否为空
    /// </summary>
    /// <returns></returns>
    bool IsEmpty()
    {
        return m_data.empty();
    }
    /// <summary>
    /// 交换两个容器的内容
    /// </summary>
    /// <param name="other">要交换的另一个容器</param>
    void Swap(Stack& other)
    {
        std::swap(m_data, other.m_data);
    }
public:
    friend bool operator==(const Stack& other)
    {
        return m_data == other.m_data;
    }

    friend bool operator!=(const Stack& other)
    {
        return m_data != other.m_data;
    }

    friend bool operator<(const Stack& other)
    {
        return m_data < other.m_data;
    }

    friend bool operator<=(const Stack& other)
    {
        return m_data <= other.m_data;
    }

    friend bool operator>(const Stack& other)
    {
        return m_data > other.m_data;
    }

    friend bool operator>=(const Stack& other)
    {
        return m_data >= other.m_data;
    }
private:
    std::stack<Type> m_data;
};