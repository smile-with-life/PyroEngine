#pragma once

#include <concepts>
#include <type_traits>

namespace Concept
{
// 整数类型
template<class Type>
concept IntegralType = std::integral<Type>;

// 浮点类型
template<class Type>
concept FloatPointType = std::floating_point<Type>;

// 算数类型
template<class Type>
concept ArithmeticType = IntegralType<Type> || FloatPointType<Type>;

// 类类型
template<class Type>
concept ClassType = std::is_class_v<Type>;

// 支持相等的可比较类型
template<class Type>
concept EqualComparableType = requires(const Type & left, const Type & right)
{
    { left == right } -> std::convertible_to<bool>;
    { left != right } -> std::convertible_to<bool>;
};

// 支持排序的可比较类型
template<class Type>
concept SortComparableType = requires(const Type & left, const Type & right)
{
    { left <  right } -> std::convertible_to<bool>;
    { left <= right } -> std::convertible_to<bool>;
    { left >  right } -> std::convertible_to<bool>;
    { left >= right } -> std::convertible_to<bool>;
};

// 可比较类型
template<class Type>
concept ComparableType = EqualComparableType<Type> && SortComparableType<Type>;

// 输出迭代器类型
template<class Type>
using OutputIteratorType = requires() {};

// 输入迭代器类型
template<class Type>
using InputIteratorType = requires() {};

// 前向迭代器类型
template<class Type>
using ForwardIteratorType = requires() {};

// 双向迭代器类型
template<class Type>
using BidirectionalIteratorType = requires() {};

// 随机迭代器类型
template<class Type>
using RandomIteratorType = requires() {};

// 连续迭代器类型
template<class Type>
using ContiguousIteratorType = requires() {}; 
}// namespace Concept