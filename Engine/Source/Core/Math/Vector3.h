#pragma once
#include"Core.h"
#include"Concept/Concept.h"

template<Concept::ArithmeticType Type>
class Vector3
{
public:
    // 默认构造函数
    Vector3()
        : x(0), y(0), z(0)
    {

    }

    // 析构函数
    ~Vector3() = default;

    // 拷贝构造函数
    Vector3(const Vector3& vec3)
        : x(vec3.x), y(vec3.y), z(vec3.z)
    {

    }

    // 拷贝赋值
    Vector3& operator=(const Vector3& vec3)
    {
        if (&vec3 == this) [[unlikely]]
        {
            return *this;
        }

        x = vec3.x;
        y = vec3.y;
        z = vec3.z;

        return *this;
    }

    // 移动构造函数
    Vector3(Vector3&& vec3) noexcept
        : x(vec3.x), y(vec3.y), z(vec3.z)
    {
        vec3.x = 0;
        vec3.y = 0;
        vec3.z = 0;
    }

    // 移动赋值
    Vector3& operator=(Vector3&& vec3) noexcept
    {
        if (&vec3 == this) [[unlikely]]
        {
            return *this;
        }
        x = vec3.x;
        y = vec3.y;
        z = vec3.z;
        vec3.x = 0;
        vec3.y = 0;
        vec3.z = 0;
        return *this;
    }

    // 列表构造函数
    Vector3& operator=(std::initializer_list<Type> vecList)
    {
        x = vecList.begin()[0];
        y = vecList.begin()[1];
        z = vecList.begin()[2];

        return *this;
    }

    // 构造函数
    explicit Vector3(Type value)
        : x(value), y(value), z(value)
    {

    }

    // 构造函数
    Vector3(Type xValue, Type yValue, Type zValue)
        : x(xValue), y(yValue), z(zValue)
    {

    }
public:
    // 向量叉积，返回一个新的向量
    Vector3 Cross(const Vector3& vec3) const
    {
        return Vector3(
            y * vec3.z - z * vec3.y,
            z * vec3.x - x * vec3.z,
            x * vec3.y - y * vec3.x
        );

    }

    // 向量点积，返回一个值
    Type Dot(const Vector3& vec3) const
    {
        return x * vec3.x + y * vec3.y + z * vec3.z;
    }

    // 获取向量的模长
    Type Length() const
    {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    // 获取向量的模长的平方
    Type SquaredLength() const
    {
        return x * x + y * y + z * z;
    }

    // 两个向量的距离
    Type Distance(const Vector3& vec3) const
    {
        return (*this - vec3).Length();
    }

    // 归一化
    bool Normalize(Type tolerance = Math::epsilon)
    {
        const Type square = x * x + y * y + z * z;
        if (square > tolerance)
        {
            const Type scale = Math::InvSqrt(square);
            x *= scale;
            y *= scale;
            z *= scale;
            return true;
        }
        return false;
    }

    // 判断是否是零向量
    bool IsZero() const
    {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }

    // 判断是否是单位向量
    bool IsUnit() const
    {
        return Math::Abs(1.0f - SquaredLength()) < 0.01f;
    }
public:
    // 向量取反
    Vector3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    // 判断两个向量是否相等
    bool operator==(const Vector3& vec3) const
    {
        return x == vec3.x && y == vec3.y && z == vec3.z;
    }

    // 判断两个向量是否不相等
    bool operator!=(const Vector3& vec3) const
    {
        return x != vec3.x || y != vec3.y || z != vec3.z;
    }

    // 两个向量相加，返回一个新的向量
    Vector3 operator+(const Vector3& vec3) const
    {
        return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
    }

    // 两个向量相加，返回它本身
    Vector3& operator+=(const Vector3& vec3)
    {
        x += vec3.x; y += vec3.y; z += vec3.z;
        return *this;
    }

    // 向量加上一个标量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector3 operator+(Scalar scale) const
    {
        return Vector3(x + (Type)scale, y + (Type)scale, z + (Type)scale);
    }

    // 向量加上一个标量，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector3& operator+=(Scalar scale)
    {
        x += scale; y += scale; z += scale;
        return *this;
    }

    // 标量加上一个向量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector3 operator+(Scalar scale, const Vector3& vec3)
    {
        return Vector3(scale + vec3.x, scale + vec3.y, scale + vec3.z);
    }

    // 两个向量相减，返回一个新的向量
    Vector3 operator-(const Vector3& vec3) const
    {
        return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
    }

    // 两个向量相减，返回它本身
    Vector3& operator-=(const Vector3& vec3)
    {
        x -= vec3.x; y -= vec3.y; z -= vec3.z;
        return *this;
    }

    // 向量减去一个标量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector3 operator-(Scalar scale) const
    {
        return Vector3(x - (Type)scale, y - (Type)scale, z - (Type)scale);
    }

    // 向量减去一个标量，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector3& operator-=(Scalar scale)
    {
        x -= scale; y -= scale; z -= scale;
        return *this;
    }

    // 标量减去一个向量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector3 operator-(Scalar scale, const Vector3& vec3)
    {
        return Vector3(scale - vec3.x, scale - vec3.y, scale - vec3.z);
    }

    // 两个向量的分量相乘，返回一个新的向量
    Vector3 operator*(const Vector3& vec3) const
    {
        return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
    }

    // 两个向量的分量相乘，返回它本身
    Vector3& operator*=(const Vector3& vec3)
    {
        x *= vec3.x; y *= vec3.y; z *= vec3.z;
        return *this;
    }

    // 向量的分量都乘以一个值，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector3 operator*(Scalar scale) const
    {
        return Vector3(x * (Type)scale, y * (Type)scale, z * (Type)scale);
    }

    // 向量的分量都乘以一个值，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector3& operator*=(Scalar scale)
    {
        x *= scale; y *= scale; z *= scale;
        return *this;
    }

    // 标量乘以一个向量的分量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector3 operator*(Scalar scale, const Vector3& vec3)
    {
        return Vector3(scale * vec3.x, scale * vec3.y, scale * vec3.z);
    }

    // 两个向量的分量相除，返回一个新的向量
    Vector3 operator/(const Vector3& vec3) const
    {
        Assert(!vec3.IsZero());
        return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
    }

    // 两个向量的分量相除，返回它本身
    Vector3& operator/=(const Vector3& vec3)
    {
        Assert(!vec3.IsZero());
        x /= vec3.x; y /= vec3.y; z /= vec3.z;
        return *this;
    }

    // 向量的分量都除以一个值，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    Vector3 operator/(Scalar scale) const
    {
        Assert(scale != 0.0);
        const Type invScale = Type(1) / scale;
        return Vector3(x * (Type)invScale, y * (Type)invScale, z * (Type)invScale);
    }

    // 向量的分量都除以一个值，返回它本身
    template<Concept::ArithmeticType Scalar>
    Vector3& operator/=(Scalar scale)
    {
        Assert(scale != 0.0);
        const Type invScale = (Type)1 / scale;
        x *= invScale; y *= invScale; z *= invScale;
        return *this;
    }

    // 标量除以一个向量的分量，返回一个新的向量
    template<Concept::ArithmeticType Scalar>
    friend Vector3 operator/(Scalar scale, const Vector3& vec3)
    {
        return Vector3(scale / vec3.x, scale / vec3.y, scale / vec3.z);
    }
public:
    union
    {
        Type data[3];
        struct { Type x, y, z; };
    };
};