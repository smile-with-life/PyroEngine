#pragma once

#include "Core.h"
#include "String/String.h"
#include "RHI/RHI.h"

#include "VulkanCommon.h"

namespace Vulkan 
{
/// <summary>
/// 获取 Vulkan API 版本字符串表示
/// </summary>
/// <returns>API 版本字符串</returns>
String APIVersion(uint32 version);

/// <summary>
/// 获取 Vulkan 物理设备类型字符串表示
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
String DeviceType(vk::PhysicalDeviceType type);

vk::Format PixelFormatVulkan(PixelFormat format);

vk::ShaderStageFlagBits ShaderTypeVulkan(ShaderUsage usage);

vk::ShaderStageFlags ConstantRangeFlagVulkan(ConstantRangeFlag rangeFlag);

vk::PrimitiveTopology PrimitiveTopologyVulkan(PrimitiveTopology topology);

vk::PolygonMode PolygonModeVulkan(PolygonMode mode);

vk::CullModeFlagBits CullModeVulkan(CullMode mode);

vk::FrontFace FrontFaceVulkan(FrontFace frontFace);

vk::SampleCountFlagBits SampleCountVulkan(SampleCount sampleCount);

vk::BufferUsageFlags BufferUsageVulkan(RHIBufferUsageFlag usageFlag);

vk::CommandBufferUsageFlagBits CommandTypeVulkan(RHICommandType type);
} // namespace Vulkan

