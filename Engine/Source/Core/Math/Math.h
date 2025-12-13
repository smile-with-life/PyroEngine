#pragma once
#include "Core.h"
#include "Math/Radian.h"
#include <cmath>
#include <limits>

namespace Math
{
// 正无穷
inline constexpr float positiveInfinity = std::numeric_limits<float>::infinity();
// 负无穷
inline constexpr float negativeInfinity = -std::numeric_limits<float>::infinity();
// 数学常数π
inline constexpr float pi = (3.1415926535897932f);
// 数学常数e
inline constexpr float e = (2.71828182845904523536f);
// 黄金分割比
inline constexpr float phi = (1.6180339887498948482045868343656381f);
// 误差值
inline constexpr float epsilon = (1.e-4f);

// 绝对值
template<Concept::ArithmeticType Type>
constexpr Type Abs(const Type value)
{
    return (value < (Type)0) ? -value : value;
}

// 最大值
template<Concept::ArithmeticType Type>
constexpr Type Max(const Type value1, const Type value2)
{
    return (value1 > value2) ? value1 : value2;
}

// 绝对值最大值
template<Concept::ArithmeticType Type>
constexpr Type AbsMax(const Type value1, const Type value2)
{
    return (Abs(value1) > Abs(value2)) ? value1 : value2;
}

// 最小值
template<Concept::ArithmeticType Type>
constexpr Type Min(const Type value1, const Type value2)
{
    return (value1 < value2) ? value1 : value2;
}

// 绝对值最小值
template<Concept::ArithmeticType Type>
constexpr Type AbsMin(const Type value1, const Type value2)
{
    return (Abs(value1) < Abs(value2)) ? value1 : value2;
}

// 计算平方根
template<Concept::FloatPointType Type>
constexpr Type Sqrt(Type value)
{
    return std::sqrt(value);
}

// 平方根的倒数
template<Concept::FloatPointType Type>
constexpr Type InvSqrt(Type value)
{
    return 1 / std::sqrt(value);
}

// 计算立方根
template<Concept::FloatPointType Type>
constexpr Type Cbrt(Type value)
{
    return std::sqrt(value);
}

// 计算立方根
template<Concept::FloatPointType Type>
constexpr Type InvCbrt(Type value)
{
    return 1 / std::sqrt(value);
}

// 幂运算
template<Concept::ArithmeticType Type>
constexpr Type Pow(Type base, Type exp)
{
    return std::pow(base, exp);
}

// log2 x 以 2 为底的对数
template<Concept::FloatPointType Type>
constexpr Type log2(Type value)
{
    return std::log2(value);
}

// log10 x 以 10 为底的对数
template<Concept::FloatPointType Type>
constexpr Type log10(Type value)
{
    return std::log10(value);
}

// ln x 以 e 为底的对数
template<Concept::FloatPointType Type>
constexpr Type ln(Type value)
{
    return std::log(value);
}

// 正弦
template<Concept::FloatPointType Type>
constexpr Type sin(const Type degree)
{
    return std::sin(degree);
}

// 余弦
template<Concept::FloatPointType Type>
constexpr Type cos(const Type degree)
{
    return std::cos(degree);
}

// 正切
template<Concept::FloatPointType Type>
constexpr Type tan(const Type degree)
{
    return std::tan(degree);
}

// 反正弦
template<Concept::FloatPointType Type>
constexpr Type arcsin(const Type degree)
{
    return std::asin(degree);
}

// 反余弦
template<Concept::FloatPointType Type>
constexpr Type arccos(const Type degree)
{
    return std::acos(degree);
}

// 反正切
template<Concept::FloatPointType Type>
constexpr Type arctan(const Type degree)
{
    return std::atan(degree);
}

// 双曲正弦
template<Concept::FloatPointType Type>
constexpr Type sinh(const Type degree)
{
    return std::sinh(degree);
}

// 双曲余弦
template<Concept::FloatPointType Type>
constexpr Type cosh(const Type degree)
{
    return std::cosh(degree);
}

// 双曲正切
template<Concept::FloatPointType Type>
constexpr Type tanh(const Type degree)
{
    return std::tanh(degree);
}

// 反双曲正弦
template<Concept::FloatPointType Type>
constexpr Type arsinh(const Type degree)
{
    return std::asinh(degree);
}

// 反双曲余弦
template<Concept::FloatPointType Type>
constexpr Type arcosh(const Type degree)
{
    return std::acosh(degree);
}

// 反双曲正切
template<Concept::FloatPointType Type>
constexpr Type artanh(const Type degree)
{
    return std::atanh(degree);
}

// 度数转弧度
template<Concept::FloatPointType Type>
constexpr Type DegToRad(Type degree)
{
    return degree / 180.0 * pi;
}

// 弧度转度数
template<Concept::FloatPointType Type>
constexpr Type RadToDeg(Type radian)
{
    return radian / pi * 180.0;
}
}// namespace Math

#include "Vector3.h"
#include "Vector4.h"