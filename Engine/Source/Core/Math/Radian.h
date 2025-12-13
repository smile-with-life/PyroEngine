#pragma once
#include "Core.h"
// 弧度
class Radian
{
public:
    Radian() = default;

    ~Radian() = default;

    Radian(const Radian& other);

    Radian& operator=(const Radian& other);

    Radian(Radian&& other) noexcept;

    Radian& operator=(Radian&& other) noexcept;

    // 构造函数
    Radian(float degree);

    Radian& operator=(float degree);
public:
    Radian operator-() const;
    Radian operator+() const;
    bool operator==(const Radian& other) const;
    bool operator!=(const Radian& other) const;
    bool operator>(const Radian& other) const;
    bool operator>=(const Radian& other) const;
    bool operator<(const Radian& other) const;
    bool operator<=(const Radian& other) const;
    Radian operator+(const Radian& other) const;
    Radian& operator+=(const Radian& other);
    Radian operator-(const Radian& other) const;
    Radian& operator-=(const Radian& other);
    Radian operator*(const Radian& other) const;
    Radian& operator*=(const Radian& other);
    Radian operator/(const Radian& other) const;
    Radian& operator/=(const Radian& other);
public:
    // 弧度值
    float m_rad = 0;
};