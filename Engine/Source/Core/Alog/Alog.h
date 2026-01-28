#pragma once
#include <algorithm>

#include "Core.h"
#include "Concept/Concept.h"

namespace Alog
{
/// <summary>
/// 返回两个值中的较大者
/// </summary>
/// <typeparam name="Type">可排序比较的类型</typeparam>
/// <param name="left">第一个比较值</param>
/// <param name="right">第二个比较值</param>
/// <returns>两个参数中的较大者。如果相等，返回第一个参数</returns>
template<Concept::SortComparableType Type>
constexpr const Type& Max(const Type& left, const Type& right)
{
    return (left > right) ? left : right;
}

/// <summary>
/// 返回初始化列表中的最大值
/// </summary>
/// <typeparam name="Type">可排序比较的类型</typeparam>
/// <param name="ilist">初始化列表</param>
/// <returns>初始化列表中的最大值</returns>
template<Concept::SortComparableType Type>
constexpr const Type Max(std::initializer_list<Type> ilist)
{
    return std::max(ilist);
}

/// <summary>
/// 返回两个值中的较小者
/// </summary>
/// <typeparam name="Type">可排序比较的类型</typeparam>
/// <param name="left">第一个比较值</param>
/// <param name="right">第二个比较值</param>
/// <returns>两个参数中的较小者。如果相等，返回第一个参数</returns>
template<Concept::SortComparableType Type>
constexpr Type Min(const Type& left, const Type& right)
{
    return (left < right) ? left : right;
}

/// <summary>
/// 返回初始化列表中的最小值
/// </summary>
/// <typeparam name="Type">可排序比较的类型</typeparam>
/// <param name="ilist">初始化列表</param>
/// <returns>初始化列表中的最小元素。</returns>
template<Concept::SortComparableType Type>
constexpr Type Min(std::initializer_list<Type> ilist)
{
    return std::min(ilist);
}

/// <summary>
/// 对范围中的每个元素执行一次给定的函数
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
template<class InputIt, class UnaryFunc>
constexpr UnaryFunc ForEach(InputIt first, InputIt last, UnaryFunc func)
{
    for (auto iter = first; iter != last; ++iter)
    {
        func(*iter);
    }
    return std::move(func);
}

/// <summary>
/// 判断范围中的所有元素是否都满足条件
/// </summary>
/// <typeparam name="InputIt">输入迭代器</typeparam>
/// <typeparam name="UnaryPred">一元谓词</typeparam>
/// <param name="first">迭代器的起始</param>
/// <param name="last">迭代器的结束</param>
/// <param name="pred">一元函数对象</param>
/// <returns>如果范围中每个元素都满足条件，则返回 true；否则返回 false。范围为空时返回 false</returns>
template<class InputIt, class UnaryPred>
constexpr bool All(InputIt first, InputIt last, UnaryPred pred)
{
    if (first == last) 
        return false;

    // 遍历范围内的所有元素
    for (auto iter = first; iter != last; ++iter)
    {
        // 如果发现一个元素不满足谓词条件，立即返回 false
        if (!pred(*iter))
        {
            return false;
        }
    }
    // 所有元素都满足条件，返回 true
    return true;
}

/// <summary>
/// 判断范围中是否存在至少一个元素满足条件
/// </summary>
/// <typeparam name="UnaryPred">一元谓词</typeparam>
/// <typeparam name="InputIt">输入迭代器类型</typeparam>
/// <param name="first">迭代器的起始</param>
/// <param name="last">迭代器的结束</param>
/// <param name="pred">一元函数对象</param>
/// <returns>如果范围中至少有一个元素满足条件，则返回 true；否则返回 false。范围为空时返回 false</returns>
template<class InputIt, class UnaryPred>
constexpr bool Any(InputIt first, InputIt last, UnaryPred pred)
{
    if (first == last)
        return false;

    // 遍历范围内的所有元素
    for (auto iter = first; iter != last; ++iter)
    {
        // 如果发现一个元素满足谓词条件，立即返回 true
        if (pred(*iter))
        {
            return true;
        }
    }
    // 没有元素满足条件，返回 false
    return false;
}

/// <summary>
/// 判断范围中的所有元素是否都不满足条件
/// </summary>
/// <typeparam name="UnaryPred">一元谓词</typeparam>
/// <typeparam name="InputIt">输入迭代器类型</typeparam>
/// <param name="first">迭代器的起始</param>
/// <param name="last">迭代器的结束</param>
/// <param name="pred">一元函数对象</param>
/// <returns>如果范围中没有任何元素满足条件，则返回 true；否则返回 false。范围为空时返回 false</returns>
template<class InputIt, class UnaryPred>
constexpr bool None(InputIt first, InputIt last, UnaryPred pred)
{
    if (first == last)
        return false;

    // 遍历范围内的所有元素
    for (auto iter = first; iter != last; ++iter)
    {
        // 如果发现一个元素满足谓词条件，立即返回 false
        if (pred(*iter))
        {
            return false;
        }
    }
    // 所有元素都不满足条件，返回 true
    return true;
}

}// namespace Alog