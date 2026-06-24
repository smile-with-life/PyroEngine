#include "pch.h"

#include "VulkanUtils.h"

namespace Vulkan 
{
String APIVersion(uint32 version)
{
    return std::format("{}.{}.{}", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
}

String DeviceType(vk::PhysicalDeviceType type)
{
    String result;
    switch (type)
    {
    case vk::PhysicalDeviceType::eCpu :
        result = "CPU Simulation";
        break;
    case vk::PhysicalDeviceType::eDiscreteGpu :
        result = "Discrete GPU";
        break;
    case vk::PhysicalDeviceType::eIntegratedGpu :
        result = "Integrated GPU";
        break;
    case vk::PhysicalDeviceType::eVirtualGpu :
        result = "Virtual GPU";
        break;
    case vk::PhysicalDeviceType::eOther :
        result = "Unknow Device";
        break;
    }
    return result;
}

vk::Format PixelFormatVulkan(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::None:                       return vk::Format::eUndefined;
    case PixelFormat::R8Unorm:                    return vk::Format::eR8Unorm;
    case PixelFormat::R8Snorm:                    return vk::Format::eR8Snorm;
    case PixelFormat::R8Uint:                     return vk::Format::eR8Uint;
    case PixelFormat::R8Sint:                     return vk::Format::eR8Sint;
    case PixelFormat::R8SRGB:                     return vk::Format::eR8Srgb;
    case PixelFormat::RG8Unorm:                   return vk::Format::eR8G8Unorm;
    case PixelFormat::RG8Snorm:                   return vk::Format::eR8G8Snorm;
    case PixelFormat::RG8Uint:                    return vk::Format::eR8G8Uint;
    case PixelFormat::RG8Sint:                    return vk::Format::eR8G8Sint;
    case PixelFormat::RG8SRGB:                    return vk::Format::eR8G8Srgb;
    case PixelFormat::RGBA8Unorm:                 return vk::Format::eR8G8B8A8Unorm;
    case PixelFormat::RGBA8Snorm:                 return vk::Format::eR8G8B8A8Snorm;
    case PixelFormat::RGBA8Uint:                  return vk::Format::eR8G8B8A8Uint;
    case PixelFormat::RGBA8Sint:                  return vk::Format::eR8G8B8A8Sint;
    case PixelFormat::RGBA8SRGB:                  return vk::Format::eR8G8B8A8Srgb;
    case PixelFormat::BGRA8Unorm:                 return vk::Format::eB8G8R8A8Unorm;
    case PixelFormat::BGRA8SRGB:                  return vk::Format::eB8G8R8A8Srgb;
    case PixelFormat::R16Float:                   return vk::Format::eR16Sfloat;
    case PixelFormat::R16Unorm:                   return vk::Format::eR16Unorm;
    case PixelFormat::R16Snorm:                   return vk::Format::eR16Snorm;
    case PixelFormat::R16Uint:                    return vk::Format::eR16Uint;
    case PixelFormat::R16Sint:                    return vk::Format::eR16Sint;
    case PixelFormat::RG16Uint:                   return vk::Format::eR16G16Uint;
    case PixelFormat::RG16Sint:                   return vk::Format::eR16G16Sint;
    case PixelFormat::RG16Float:                  return vk::Format::eR16G16Sfloat;
    case PixelFormat::RGBA16Float:                return vk::Format::eR16G16B16A16Sfloat;
    case PixelFormat::R32Float:                   return vk::Format::eR32Sfloat;
    case PixelFormat::R32Uint:                    return vk::Format::eR32Uint;
    case PixelFormat::R32Sint:                    return vk::Format::eR32Sint;
    case PixelFormat::RG32Float:                  return vk::Format::eR32G32Sfloat;
    case PixelFormat::RGBA32Float:                return vk::Format::eR32G32B32A32Sfloat;
    case PixelFormat::RGB10A2Unorm:               return vk::Format::eA2R10G10B10UnormPack32;
    case PixelFormat::RGB10A2Uint:                return vk::Format::eA2R10G10B10UintPack32;
    case PixelFormat::R11G11B10Float:             return vk::Format::eB10G11R11UfloatPack32;
    case PixelFormat::R9G9B9E5Float:              return vk::Format::eE5B9G9R9UfloatPack32;
    case PixelFormat::Depth16Unorm:               return vk::Format::eD16Unorm;
    case PixelFormat::Depth24UnormStencil8:       return vk::Format::eD24UnormS8Uint;
    case PixelFormat::Depth32Float:               return vk::Format::eD32Sfloat;
    case PixelFormat::Depth32FloatStencil8:       return vk::Format::eD32SfloatS8Uint;
    case PixelFormat::BC1Unorm:                   return vk::Format::eBc1RgbUnormBlock;
    case PixelFormat::BC1SRGB:                    return vk::Format::eBc1RgbSrgbBlock;
    case PixelFormat::BC2Unorm:                   return vk::Format::eBc2UnormBlock;
    case PixelFormat::BC2SRGB:                    return vk::Format::eBc2SrgbBlock;
    case PixelFormat::BC3Unorm:                   return vk::Format::eBc3UnormBlock;
    case PixelFormat::BC3SRGB:                    return vk::Format::eBc3SrgbBlock;
    case PixelFormat::BC4Unorm:                   return vk::Format::eBc4UnormBlock;
    case PixelFormat::BC4Snorm:                   return vk::Format::eBc4SnormBlock;
    case PixelFormat::BC5Unorm:                   return vk::Format::eBc5UnormBlock;
    case PixelFormat::BC5Snorm:                   return vk::Format::eBc5SnormBlock;
    case PixelFormat::BC6HSFloat:                 return vk::Format::eBc6HSfloatBlock;
    case PixelFormat::BC7Unorm:                   return vk::Format::eBc7UnormBlock;
    case PixelFormat::BC7SRGB:                    return vk::Format::eBc7SrgbBlock;
    case PixelFormat::ASTC4x4Unorm:               return vk::Format::eAstc4x4UnormBlock;
    case PixelFormat::ASTC4x4SRGB:                return vk::Format::eAstc4x4SrgbBlock;
    case PixelFormat::ASTC6x6Unorm:               return vk::Format::eAstc6x6UnormBlock;
    case PixelFormat::ASTC6x6SRGB:                return vk::Format::eAstc6x6SrgbBlock;
    case PixelFormat::ASTC8x8Unorm:               return vk::Format::eAstc8x8UnormBlock;
    case PixelFormat::ASTC8x8SRGB:                return vk::Format::eAstc8x8SrgbBlock;
    case PixelFormat::ETC2RGB8Unorm:              return vk::Format::eEtc2R8G8B8UnormBlock;
    case PixelFormat::ETC2RGB8SRGB:               return vk::Format::eEtc2R8G8B8SrgbBlock;
    case PixelFormat::ETC2RGBA8Unorm:             return vk::Format::eEtc2R8G8B8A8UnormBlock;
    case PixelFormat::ETC2RGBA8SRGB:              return vk::Format::eEtc2R8G8B8A8SrgbBlock;
    case PixelFormat::ETC2EACR11Unorm:            return vk::Format::eEacR11UnormBlock;
    case PixelFormat::ETC2EACR11Snorm:            return vk::Format::eEacR11SnormBlock;
    case PixelFormat::ETC2EACRG11Unorm:           return vk::Format::eEacR11G11UnormBlock;
    case PixelFormat::ETC2EACRG11Snorm:           return vk::Format::eEacR11G11SnormBlock;
    default:                                      return vk::Format::eUndefined;
    }
}

vk::ShaderStageFlagBits ShaderTypeVulkan(ShaderUsage usage)
{
    switch (usage)
    {
    case ShaderUsage::Vertex:
        return vk::ShaderStageFlagBits::eVertex;
    case ShaderUsage::Mesh:
        return vk::ShaderStageFlagBits::eMeshEXT;
    case ShaderUsage::Amplification:
        return vk::ShaderStageFlagBits::eTaskEXT;
    case ShaderUsage::Pixel:
        return vk::ShaderStageFlagBits::eFragment;
    case ShaderUsage::Geometry:
        return vk::ShaderStageFlagBits::eGeometry;
    case ShaderUsage::Compute:
        return vk::ShaderStageFlagBits::eCompute;
    default:
        throw std::runtime_error("Shader Usage Undefinded!");
    }
}

vk::ShaderStageFlags ConstantRangeFlagVulkan(ConstantRangeFlag rangeFlag)
{
    auto HasFlag = [](ConstantRangeFlag u, ConstantRangeFlag f) -> bool
    {
        return (static_cast<uint32>(u) & static_cast<uint32>(f)) != 0;
    };
    vk::ShaderStageFlags flags = {};
    if (HasFlag(rangeFlag, ConstantRangeFlag::Vertex))
    {
        flags |= vk::ShaderStageFlagBits::eVertex;
    }
    if (HasFlag(rangeFlag, ConstantRangeFlag::Pixel))
    {
        flags |= vk::ShaderStageFlagBits::eFragment;
    }
    if (HasFlag(rangeFlag, ConstantRangeFlag::Compute))
    {
        flags |= vk::ShaderStageFlagBits::eCompute;
    }
    return flags;
}

vk::PrimitiveTopology PrimitiveTopologyVulkan(PrimitiveTopology topology)
{
    switch (topology)
    {
    case PrimitiveTopology::PointList :
        return vk::PrimitiveTopology::ePointList;
    case PrimitiveTopology::LineList :
        return vk::PrimitiveTopology::eLineList;
    case PrimitiveTopology::LineStrip :
        return vk::PrimitiveTopology::eLineStrip;
    case PrimitiveTopology::TriangleList:
        return vk::PrimitiveTopology::eTriangleList;
    case PrimitiveTopology::TriangleStrip:
        return vk::PrimitiveTopology::eTriangleStrip;
    case PrimitiveTopology::PatchList:
        return vk::PrimitiveTopology::ePatchList;
    }
}

vk::PolygonMode PolygonModeVulkan(PolygonMode mode)
{
    switch (mode)
    {
    case PolygonMode::Fill:
        return vk::PolygonMode::eFill;
    case PolygonMode::Line:
        return vk::PolygonMode::eLine;
    case PolygonMode::Point:
        return vk::PolygonMode::ePoint;
    }
}

vk::CullModeFlagBits CullModeVulkan(CullMode mode)
{
    switch (mode)
    {
    case CullMode::None:
        return vk::CullModeFlagBits::eNone;
    case CullMode::Front:
        return vk::CullModeFlagBits::eFront;
    case CullMode::Back:
        return vk::CullModeFlagBits::eBack;
    }
}

vk::FrontFace FrontFaceVulkan(FrontFace frontFace)
{
    switch (frontFace)
    {
    case FrontFace::Clockwise:
        return vk::FrontFace::eClockwise;
    case FrontFace::CounterClockwise:
        return vk::FrontFace::eCounterClockwise;
    }
}

vk::SampleCountFlagBits SampleCountVulkan(SampleCount sampleCount)
{
    switch (sampleCount)
    {
    case SampleCount::X1:
        return vk::SampleCountFlagBits::e1;
    case SampleCount::X2:
        return vk::SampleCountFlagBits::e2;
    case SampleCount::X4:
        return vk::SampleCountFlagBits::e4;
    case SampleCount::X8:
        return vk::SampleCountFlagBits::e8;
    }
}

vk::BufferUsageFlags BufferUsageVulkan(RHIBufferUsageFlag usageFlag)
{
    auto HasFlag = [](RHIBufferUsageFlag u, RHIBufferUsageFlag f) -> bool
    {
        return (static_cast<uint32>(u) & static_cast<uint32>(f)) != 0;
    };
    vk::BufferUsageFlags flags = {};
    if (HasFlag(usageFlag, RHIBufferUsageFlag::Vertex))
    {
        flags |= vk::BufferUsageFlagBits::eVertexBuffer;
    }
    if (HasFlag(usageFlag, RHIBufferUsageFlag::Index))
    {
        flags |= vk::BufferUsageFlagBits::eIndexBuffer;
    }
    if (HasFlag(usageFlag, RHIBufferUsageFlag::Uniform))
    {
        flags |= vk::BufferUsageFlagBits::eUniformBuffer;
    }
    if (HasFlag(usageFlag, RHIBufferUsageFlag::Storage))
    {
        flags |= vk::BufferUsageFlagBits::eStorageBuffer;
    }
    if (HasFlag(usageFlag, RHIBufferUsageFlag::Indirect))
    {
        flags |= vk::BufferUsageFlagBits::eIndirectBuffer;
    }

    // 通用传输,
    // 所有 Buffer 默认支持 Shader 地址
    flags |= vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress;

    return flags;
}

vk::CommandBufferUsageFlagBits CommandTypeVulkan(RHICommandType type)
{
    switch (type)
    {
    case RHICommandType::Single :
        return vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    case RHICommandType::Simultaneous :
        return vk::CommandBufferUsageFlagBits::eSimultaneousUse;
    }
}

} // namespace Vulkan