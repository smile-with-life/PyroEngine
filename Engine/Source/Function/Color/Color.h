#pragma once
#include "Core.h"
#include "Concept/Concept.h"
#include "Math/Math.h"

/// <summary>
/// 伽马空间类型
/// </summary>
enum class GammaSpace : uint8
{
    None,       // 未知
    Linear,     // 线性空间
    Pow22,      // 简化伽马空间
    sRGB,       // 标准伽马空间
};

/// <summary>
/// 高精度颜色类
/// <para>适用于渲染计算、HDR 流程及线性空间运算</para>
/// <para>存储类型：32 位浮点数</para>
/// <para>取值范围：0.0 ~ 1.0（支持超范围如 HDR 值 > 1.0）</para>
/// <para>伽马空间：线性空间</para>
/// </summary>
class StandardColor
{
public:
    StandardColor();

    ~StandardColor();

    StandardColor(const StandardColor& other);

    StandardColor& operator=(const StandardColor& other);

    StandardColor(StandardColor&& other) noexcept;

    StandardColor& operator=(StandardColor&& other) noexcept;

    StandardColor(float r, float g, float b, float a = 1.0f);

    explicit StandardColor(const Color& color);

    template<Concept::FloatPointType Type>
    explicit StandardColor(const Vector3<Type>& color);

    template<Concept::FloatPointType Type>
    explicit StandardColor(const Vector4<Type>& color);
public:

public:
    static StandardColor FromRGB(float r, float g, float b);

    static StandardColor FromRGBA(float r, float g, float b, float a);

    static StandardColor FromColor(const Color& color);
public:
    float Red;
    float Green;
    float Blue;
    float Alpha;
};

/// <summary>
/// 颜色类
/// <para>适用于存储、传输和伽马空间操作</para>
/// <para>存储类型：8 位无符号整数</para>
/// <para>取值范围：0 ~ 255</para>
/// <para>伽马空间：标准伽马空间</para>
/// </summary>
class Color
{
public:
    Color() = default;

    ~Color() = default;

    Color(const Color& other) = default;

    Color& operator=(const Color& other) = default;

    Color(Color&& other) noexcept = default;

    Color& operator=(Color&& other) noexcept = default;

    /// <summary>
    /// 从HTML颜色码（支持 #RRGGBB 或 #RRGGBBAA 格式）
    /// </summary>
    /// <param name="hex"></param>
    explicit Color(const String& hex);

    explicit Color(const StandardColor& standardColor);

    template<Concept::IntegralType Type>
    explicit Color(const Vector3<Type>& color);

    template<Concept::IntegralType Type>
    explicit Color(const Vector4<Type>& color); 

    Color(int8 r, int8 g, int8 b, int8 a = 255);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    StandardColor ToStandardColor() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    String ToHex() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint32 ToUInt32ARGB() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint32 ToUInt32BGRA() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint32 ToUInt32RGBA() const;
public:
    static Color FromRGB(int8 r, int8 g, int8 b);

    static Color FromRGBA(int8 r, int8 g, int8 b, int8 a);

    static Color FromHex(const String& hex);

    static Color FromStandardColor(const StandardColor& standardColor);

    static Color FromUInt32ARGB(uint32 argb);

    static Color FromUInt32BGRA(uint32 bgra);

    static Color FromUInt32RGBA(uint32 rgba);
public:
    uint8 Red;
    uint8 Green;
    uint8 Blue;
    uint8 Alpha;
};