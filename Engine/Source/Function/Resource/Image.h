#pragma once
#include "Core.h"
#include "String/String.h"

// 图像格式
enum class ImageFormat : uint16
{
    None,
    R8,
    RG8,
    RGB8,
    RGBA8,
};


// 图像
class Image
{
public:
    Image() = default;

    ~Image();

    Image(const Image& other) = delete;

    Image& operator=(const Image& other) = delete;

    Image(Image&& other) noexcept;

    Image& operator=(Image&& other) noexcept;

    Image(const String& path);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="path"></param>
    void Load(const String& path);

    /// <summary>
    /// 
    /// </summary>
    void Clear();

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint32 Width() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint32 Height() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    ImageFormat Format() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    uint8* Data();

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    const uint8* Data() const;
private: 
    /// <summary>
    /// 图像的宽度，以像素为单位
    /// </summary>
    uint32 m_width = 0;

    /// <summary>
    /// 图像的高度，以像素为单位
    /// </summary>
    uint32 m_height = 0;

    /// <summary>
    /// 图像大小
    /// </summary>
    uint32 m_size = 0;

    /// <summary>
    /// 图像的格式
    /// </summary>
    ImageFormat m_format = ImageFormat::None;

    /// <summary>
    /// 数据
    /// </summary>
    uint8* m_data = nullptr;
};