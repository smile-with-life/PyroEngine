#pragma once
#include <algorithm>

#include "Core.h"
#include "Concept/Concept.h"

namespace Alog
{
// 两个参数中的最大值
template<Concept::SortComparableType Type>
constexpr const Type& Max(const Type& a, const Type& b)
{
    return (a > b) ? a : b;
}

// 列表中的最大值
template<Concept::SortComparableType Type>
constexpr const Type Max(std::initializer_list<Type> ilist)
{
    return std::max(ilist);
}

// 两个参数中的最小值
template<Concept::SortComparableType Type>
constexpr Type& Min(const Type& a, const Type& b)
{
    return (a < b) ? a : b;
}

// 列表中的最小值
template<Concept::SortComparableType Type>
constexpr Type Min(std::initializer_list<Type> ilist)
{
    return std::min(ilist);
}


/// <summary>
/// 遍历容器
/// <para>
/// func 可以是一个有状态的函数对象。返回值可以被视为批处理操作的最终状态
/// </para>
/// </summary>
/// <typeparam name="InputIt">输入迭代器</typeparam>
/// <typeparam name="UnaryFunc">一元函数</typeparam>
/// <param name="first">迭代器的起始</param>
/// <param name="last">迭代器的结束</param>
/// <param name="func">一元函数对象（应是可移动构造的）</param>
/// <returns>传入的一元函数对象</returns>
template<Concept::InputIteratorType InputIt, class UnaryFunc>
constexpr UnaryFunc ForEach(InputIt first, InputIt last, UnaryFunc func)
{
    for (auto iter = first; iter != last; ++iter)
    {
        func(*iter);
    }
    return std::move(func);
}

template<Concept::InputIteratorType InputIt, class UnaryPred>
constexpr bool All(InputIt first, InputIt last, UnaryPred perd)
{

    // @实现
    return false;
}

template<Concept::InputIteratorType InputIt, class UnaryPred>
constexpr bool Any(InputIt first, InputIt last, UnaryPred perd)
{
    // @实现
    return false;
}

template<Concept::InputIteratorType InputIt, class UnaryPred>
constexpr bool None(InputIt first, InputIt last, UnaryPred perd)
{
    // @实现
    return false;
}




}// namespace Alog