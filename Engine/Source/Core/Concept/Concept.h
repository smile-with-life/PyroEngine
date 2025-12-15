#pragma once

#include <concepts>
#include <type_traits>
#include <iterator>

namespace Concept
{
/* ==================== 核心语言概念 ==================== */

// 概念：带符号的整数类型
template<class Type>
concept SignedIntegralType = std::signed_integral<Type>;

// 概念：无符号的整数类型
template<class Type>
concept UnsignedIntegralType = std::unsigned_integral<Type>;

// 概念：整数类型
template<class Type>
concept IntegralType = std::integral<Type>;

// 概念：浮点类型
template<class Type>
concept FloatPointType = std::floating_point<Type>;

// 概念：算数类型
template<class Type>
concept ArithmeticType = IntegralType<Type> || FloatPointType<Type>;

// 概念：类类型
template<class Type>
concept ClassType = std::is_class_v<Type>;

// 概念：可交换类型
template<class Type>
concept SwapableType = requires(Type & left, Type & right)
{
    { std::swap(left, right) } noexcept;
};

// 概念：与目标类型相同
template<class T1, class T2>
concept SameAs = std::same_as<T1, T2>;

// 概念：派生自目标类型
template<class Derived, class Base>
concept DerivedFrom = std::derived_from<Base, Derived>;

// 概念：可隐式转换为目标类型
template<class From, class To>
concept ConvertibleTo = std::convertible_to<From, To>;

// 概念：共享一个共同的类型的两种类型
template<class T1, class T2>
concept CommonWith = std::common_with<T1, T2>;

// 概念：共享一个共同的引用类型的两种类型
template<class T1, class T2>
concept CommonReferenceWith = std::common_with<T1, T2>;

// 概念：可从指定类型赋值
template<class T1, class T2>
concept AssignableFrom = std::assignable_from<T1, T2>;

//  概念：可从一组参数构造
template<class Type, class... Args>
concept ConstructibleFrom = std::constructible_from<Type, Args...>;

// 概念：可交换的两种类型
template<class T1, class T2>
concept SwapableWith = std::swappable_with<T1, T2>;

/* ==================== 比较概念 ==================== */

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

// 支持转换为 bool 的类型
template<class Type>
concept BoolableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};

/* ==================== 对象概念 ==================== */

// 概念：具有默认构造函数的类型
template<class Type>
concept DefaultInitializeType = std::default_initializable<Type>;

// 概念：具有移动构造函数的类型
template<class Type>
concept MoveConstructType = std::move_constructible<Type>;

// 概念：具有拷贝构造函数的类型(@修改)
template<class Type>
concept CopyConstructType = std::copy_constructible<Type>;

// 概念：具有移动赋值函数的类型
template<class Type>
concept MoveAssignType = std::assignable_from<Type&, Type&&>;

// 概念：具有拷贝赋值函数的类型
template<class Type>
concept CopyAssignType = std::assignable_from<Type&, Type&> && std::assignable_from<Type&,const Type&>;

// 概念：可移动的类型（移动构造、移动赋值、交换）
template<class Type>
concept MoveableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};

// 概念：可拷贝的类型（拷贝构造、拷贝赋值、交换）
template<class Type>
concept CopyableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};


// 迭代器类型
template<class Iter>
concept IteratorType = requires(Iter iter) 
{
    typename std::iter_value_t<Iter>;     // 必须有值类型
    typename std::iter_reference_t<Iter>; // 必须有引用类型
    { *iter } -> std::same_as<std::iter_reference_t<Iter>>; // 必须可解引用
    { ++iter } -> std::same_as<Iter&>;    // 必须可递增
};
//
// 输出迭代器类型
template<class Iter, class Type>
concept OutputIteratorType = IteratorType<Iter> && requires(Iter iter, std::iter_value_t<Iter> value) 
{
    *iter = std::move(value);  // 必须可赋值
};

// 输入迭代器类型
template<class Iter>
concept InputIteratorType = IteratorType<Iter> && requires(Iter iter) 
{
    { --iter } -> std::same_as<Iter&>;    // 必须可递减
};
//
//// 前向迭代器类型
//template<class Iter, class Type>
//concept ForwardIteratorType = requires(Iter iter, Type value)
//{
//    *iter = value;  // 必须可写
//    ++iter;         // 必须可递增
//};
//
//// 双向迭代器类型
//template<class Iter, class Type>
//concept BidirectionalIteratorType = requires(Iter iter, Type value)
//{
//    *iter = value;  // 必须可写
//    ++iter;         // 必须可递增
//};
//
//// 随机迭代器类型
//template<class Iter, class Type>
//concept RandomIteratorType = requires(Iter iter, Type value)
//{
//    *iter = value;  // 必须可写
//    ++iter;         // 必须可递增
//};
//
//// 连续迭代器类型
//template<class Iter, class Type>
//concept ContiguousIteratorType = requires(Iter iter, Type value)
//{
//    *iter = value;  // 必须可写
//    ++iter;         // 必须可递增
//};
}// namespace Concept