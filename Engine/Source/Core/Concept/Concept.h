#pragma once

#include <concepts>

namespace Concept
{
/* ==================== 核心语言概念 ==================== */

/// <summary>
/// 概念：带符号的整数类型
/// </summary>
template<class Type>
concept SignedIntegralType = std::signed_integral<Type>;

/// <summary>
/// 概念：无符号的整数类型
/// </summary>
template<class Type>
concept UnsignedIntegralType = std::unsigned_integral<Type>;

/// <summary>
/// 概念：整数类型
/// </summary>
template<class Type>
concept IntegralType = std::integral<Type>;

/// <summary>
/// 概念：浮点类型
/// </summary>
template<class Type>
concept FloatPointType = std::floating_point<Type>;

/// <summary>
/// 概念：算数类型
/// </summary>
template<class Type>
concept ArithmeticType = IntegralType<Type> || FloatPointType<Type>;

/// <summary>
/// 概念：类类型
/// </summary>
template<class Type>
concept ClassType = std::is_class_v<Type>;

/// <summary>
/// 概念：可交换类型
/// </summary>
template<class Type>
concept SwapableType = requires(Type & left, Type & right)
{
    { std::swap(left, right) } noexcept;
};

/// <summary>
/// 概念：可以被销毁的类型
/// </summary>
template<class Type>
concept DestructibleType = std::destructible<Type>;

/// <summary>
/// 概念：与目标类型相同
/// </summary>
template<class T1, class T2>
concept SameAs = std::same_as<T1, T2>;

/// <summary>
/// 概念：派生自目标类型
/// </summary>
template<class Derived, class Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

/// <summary>
/// 概念：可隐式转换为目标类型
/// </summary>
template<class From, class To>
concept ConvertibleTo = std::convertible_to<From, To>;

/// <summary>
/// 概念：共享一个共同的类型的两种类型
/// </summary>
template<class T1, class T2>
concept CommonWith = std::common_with<T1, T2>;

/// <summary>
/// 概念：共享一个共同的引用类型的两种类型
/// </summary>
template<class T1, class T2>
concept CommonReferenceWith = std::common_with<T1, T2>;

/// <summary>
/// 概念：可从指定类型赋值
/// </summary>
template<class T1, class T2>
concept AssignableFrom = std::assignable_from<T1, T2>;

/// <summary>
/// 概念：可从一组参数构造
/// </summary>
template<class Type, class... Args>
concept ConstructibleFrom = std::constructible_from<Type, Args...>;

/// <summary>
/// 概念：可交换的两种类型
/// </summary>
template<class T1, class T2>
concept SwapableWith = std::swappable_with<T1, T2>;

/* ==================== 对象概念 ==================== */

/// <summary>
/// 支持相等的可比较类型
/// </summary>
template<class Type>
concept EqualComparableType = requires(const Type & left, const Type & right)
{
    { left == right } -> std::convertible_to<bool>;
    { left != right } -> std::convertible_to<bool>;
};

/// <summary>
/// 支持排序的可比较类型
/// </summary>
template<class Type>
concept SortComparableType = requires(const Type & left, const Type & right)
{
    { left < right } -> std::convertible_to<bool>;
    { left <= right } -> std::convertible_to<bool>;
    { left > right } -> std::convertible_to<bool>;
    { left >= right } -> std::convertible_to<bool>;
};

/// <summary>
/// 可比较类型
/// </summary>
template<class Type>
concept ComparableType = EqualComparableType<Type> && SortComparableType<Type>;

/// <summary>
/// 支持转换为 bool 的类型
/// </summary>
template<class Type>
concept BoolableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};

/// <summary>
/// 概念：具有默认构造函数的类型
/// </summary>
template<class Type>
concept DefaultInitializeType = std::default_initializable<Type>;

/// <summary>
/// 概念：具有移动构造函数的类型
/// </summary>
template<class Type>
concept MoveConstructType = std::move_constructible<Type>;

/// <summary>
/// 概念：具有拷贝构造函数的类型
/// </summary>
template<class Type>
concept CopyConstructType = std::copy_constructible<Type>;

/// <summary>
/// 概念：具有移动赋值函数的类型
/// </summary>
template<class Type>
concept MoveAssignType = std::assignable_from<Type&, Type&&>;

/// <summary>
/// 概念：具有拷贝赋值函数的类型
/// </summary>
template<class Type>
concept CopyAssignType = std::assignable_from<Type&, Type&>&& std::assignable_from<Type&, const Type&>;

/// <summary>
/// 概念：可移动的类型（移动构造、移动赋值、交换）
/// </summary>
template<class Type>
concept MoveableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};

/// <summary>
/// 概念：可拷贝的类型（拷贝构造、拷贝赋值、移动构造、移动赋值、交换）
/// </summary>
template<class Type>
concept CopyableType = requires(const Type & obj)
{
    { !std::forward<Type>(obj) } -> std::convertible_to<bool>;
};

}// namespace Concept