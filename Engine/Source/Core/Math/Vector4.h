#pragma once
#include"Core.h"
#include"Concept/Concept.h"

#include <initializer_list>

template<Concept::ArithmeticType Type>
class Vector4
{
public:
    // 默认构造函数
    Vector4()
        : x(0), y(0), z(0), w(0)
    {

    }

    // 析构函数
    ~Vector4() = default;

    // 拷贝构造函数
    Vector4(const Vector4& vec4)
        : x(vec4.x), y(vec4.y), z(vec4.z), w(vec4.w)
    {

    }

    // 拷贝赋值
    Vector4& operator=(const Vector4& vec4)
    {
        if (&vec4 == this) [[unlikely]]
        {
            return *this;
        }
        x = vec4.x;
        y = vec4.y;
        z = vec4.z;
        w = vec4.w;
        return *this;
    }

    // 移动构造函数
    Vector4(Vector4&& vec4)
        : x(vec4.x), y(vec4.y), z(vec4.z), w(vec4.w)
    {
        vec4.x = 0;
        vec4.y = 0;
        vec4.z = 0;
        vec4.w = 0;
    }

    // 移动赋值
    Vector4& operator=(Vector4&& vec4)
    {
        if (&vec4 == this) [[unlikely]]
        {
            return *this;
        }
        x = vec4.x;
        y = vec4.y;
        z = vec4.z;
        w = vec4.w;
        vec4.x = 0;
        vec4.y = 0;
        vec4.z = 0;
        vec4.w = 0;
        return *this;
    }

    // 列表构造函数
    Vector4& operator=(std::initializer_list<Type> vecList)
    {
        x = vecList.begin()[0];
        y = vecList.begin()[1];
        z = vecList.begin()[2];
        w = vecList.begin()[3];
        return *this;
    }

    // 构造函数
    explicit Vector4(Type value)
        : x(value), y(value), z(value), w(value)
    {

    }

    // 构造函数
    Vector4(Type xValue, Type yValue, Type zValue, Type wValue)
        : x(xValue), y(yValue), z(zValue), w(wValue)
    {

    }
public:
    Type Dot(const Vector4& vec4) const
    {
        return x * vec4.x + y * vec4.y + z * vec4.z + w * vec4.w;
    }

    // 判断是否是零向量
    bool IsZero() const
    {
        return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
    }
public:
    // 向量取反
    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    // 判断两个向量是否相等
    bool operator==(const Vector4& vec4) const
    {
        return x == vec4.x && y == vec4.y && z == vec4.z && w == vec4.w;
    }

    // 判断两个向量是否不相等
    bool operator!=(const Vector4& vec4) const
    {
        return x != vec4.x || y != vec4.y || z != vec4.z || w != vec4.w;
    }

    // 向量加法，返回一个新的向量
    Vector4 operator+(const Vector4& vec4) const
    {
        return Vector4(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
    }

    // 向量加法，返回它本身
    Vector4& operator+=(const Vector4& vec4)
    {
        x += vec4.x; y += vec4.y; z += vec4.z; w += vec4.w;
        return *this;
    }

    // 向量的每个分量都加上一个标量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector4 operator+(Scalar scale) const
    {
        return Vector4(x + (Type)scale, y + (Type)scale, z + (Type)scale, w + (Type)scale);
    }

    // 向量的每个分量都加上一个值，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector4& operator+=(Scalar scale)
    {
        x += scale; y += scale; z += scale; w += scale;
        return *this;
    }

    // 标量加上一个向量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector4 operator+(Scalar scale, const Vector4& vec4)
    {
        return Vector4(scale + vec4.x, scale + vec4.y, scale + vec4.z, scale + vec4.w);
    }

    // 向量减法，返回一个新的向量
    Vector4 operator-(const Vector4& vec4) const
    {
        return Vector4(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
    }

    // 向量减法，返回它本身
    Vector4& operator-=(const Vector4& vec4)
    {
        x -= vec4.x; y -= vec4.y; z -= vec4.z; w -= vec4.w;
        return *this;
    }

    // 向量的每个分量都减去一个标量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector4 operator-(Scalar scale) const
    {
        return Vector4(x - (Type)scale, y - (Type)scale, z - (Type)scale, w - (Type)scale);
    }

    // 向量的每个分量都减去一个值，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector4& operator-=(Scalar scale)
    {
        x -= scale; y -= scale; z -= scale; w -= scale;
        return *this;
    }

    // 标量减去一个向量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector4 operator-(Scalar scale, const Vector4& vec4)
    {
        return Vector4(scale - vec4.x, scale - vec4.y, scale - vec4.z, scale - vec4.w);
    }

    // 向量与另一个向量的分量相乘，返回一个新的向量
    Vector4 operator*(const Vector4& vec4) const
    {
        return Vector4(x * vec4.x, y * vec4.y, z * vec4.z, w * vec4.w);
    }

    // 向量与另一个向量的分量相乘，返回它本身
    Vector4& operator*=(const Vector4& vec4)
    {
        x *= vec4.x; y *= vec4.y; z *= vec4.z; w *= vec4.w;
        return *this;
    }

    // 向量的每个分量都乘以一个值，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector4 operator*(Scalar scale) const
    {
        return Vector4(x * (Type)scale, y * (Type)scale, z * (Type)scale, w * (Type)scale);
    }

    // 向量的每个分量都乘以一个值，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector4<Type>& operator*=(Scalar scale)
    {
        x *= scale; y *= scale; z *= scale; w *= scale;
        return *this;
    }

    // 标量乘以一个向量的分量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector4 operator*(Scalar scale, const Vector4& vec4)
    {
        return Vector4(scale * vec4.x, scale * vec4.y, scale * vec4.z, scale * vec4.w);
    }

    // 向量与另一个向量的分量相除，返回一个新的向量
    Vector4 operator/(const Vector4& vec4) const
    {
        Assert(!vec4.IsZero());
        return Vector4(x / vec4.x, y / vec4.y, z / vec4.z, w / vec4.w);
    }

    // 向量与另一个向量的分量相除，返回它本身
    Vector4& operator/=(const Vector4& vec4)
    {
        Assert(!vec4.IsZero());
        x /= vec4.x; y /= vec4.y; z /= vec4.z; w /= vec4.w;
        return *this;
    }

    // 获取向量的每个分量除以一个值的结果，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector4 operator/(Scalar scale) const
    {
        Assert(scale != 0.0);
        const Type invScale = Type(1) / scale;
        return Vector4(x * (Type)invScale, y * (Type)invScale, z * (Type)invScale, w * (Type)invScale);
    }

    // 获取向量的每个分量除以一个值的结果，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector4<Type>& operator/=(Scalar scale)
    {
        Assert(scale != 0.0);
        const Type invScale = (Type)1 / scale;
        x *= invScale; y *= invScale; z *= invScale; w *= invScale;
        return *this;
    }

    // 标量除以一个向量的分量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector4 operator/(Scalar scale, const Vector4& vec4)
    {
        return Vector4(scale / vec4.x, scale / vec4.y, scale / vec4.z, scale / vec4.w);
    }
public:
    union
    {
        Type data[4];
        struct { Type x, y, z, w; };
    };
};