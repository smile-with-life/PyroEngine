#include "pch.h"

#include "VulkanBuffer.h"

#include "Logger/Logger.h"

#include <vma/vk_mem_alloc.h>

auto& Allocator = VulkanRHI::GetInstance().Allocator;

static auto& Device = VulkanRHI::GetInstance().Device;

VulkanStaticBuffer::VulkanStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
    : RHIStaticBuffer(usageFlag, capacity)
{
    VkBuffer buffer;

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = capacity;
    bufferCreateInfo.usage = (VkBufferUsageFlags)Vulkan::BufferUsageVulkan(m_usageFlag);
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo bufferAllocCreateInfo = {};
    bufferAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    bufferAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;

    auto result = vmaCreateBuffer(Allocator, &bufferCreateInfo, &bufferAllocCreateInfo, &buffer, &m_bufferAllocation, &m_bufferAllocationInfo);
    if (result != VK_SUCCESS)
    {
        GLog->Error("VMA Alloc Static Buffer Failed! Error Code: {0}");
        throw std::runtime_error("VMA Alloc Static Buffer Failed!");
    }

    m_buffer = buffer;
}

VulkanStaticBuffer::~VulkanStaticBuffer()
{
    Destroy();
}

void VulkanStaticBuffer::Upload(const void* data, uint64 size, uint64 offset)
{
    // 1. 创建 Staging 缓冲 (CPU 可见)
    VkBufferCreateInfo stagingInfo{};
    stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingInfo.size = size;
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingAllocInfo{};
    stagingAllocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

    VkBuffer stagingBuffer;
    VmaAllocation stagingAlloc;
    if (vmaCreateBuffer(Allocator, &stagingInfo, &stagingAllocInfo, &stagingBuffer, &stagingAlloc, nullptr) != VK_SUCCESS)
        throw std::runtime_error("Failed to create staging buffer");

    // 2. 映射并拷贝数据
    void* mappedData = nullptr;
    vmaMapMemory(Allocator, stagingAlloc, &mappedData);
    memcpy(mappedData, data, size);
    vmaUnmapMemory(Allocator, stagingAlloc);

    // 3. 执行 GPU 拷贝
    //VkCommandBuffer cmd = Device.BeginSingleTimeCommands();
    //VkBufferCopy copyRegion{};
    //copyRegion.srcOffset = 0;
    //copyRegion.dstOffset = offset;
    //copyRegion.size = size;
    //vkCmdCopyBuffer(cmd, stagingBuffer, m_buffer, 1, &copyRegion);
    //m_device->EndSingleTimeCommands(cmd);

    // 4. 销毁 Staging
    vmaDestroyBuffer(Allocator, stagingBuffer, stagingAlloc);
}

uint64 VulkanStaticBuffer::GetDeviceAddress() const
{
    vk::BufferDeviceAddressInfo info;
    info.buffer = m_buffer;
    return (uint64)Device.getBufferAddress(info);
}

void VulkanStaticBuffer::Destroy()
{
    if (Device)
    {
        vmaDestroyBuffer(Allocator, m_buffer, m_bufferAllocation);
    }
}

void* VulkanStaticBuffer::GetNativeHandle()
{
    return &m_buffer;
}

VulkanDynamicBuffer::VulkanDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
    : RHIDynamicBuffer(usageFlag, capacity)
{
    VkBuffer buffer;
    VkBufferCreateInfo bufInfo{};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = m_capacity;
    bufInfo.usage = (VkBufferUsageFlags)Vulkan::BufferUsageVulkan(m_usageFlag);
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // VMA 分配信息：动态 = CPU可写、连续写
    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        | VMA_ALLOCATION_CREATE_MAPPED_BIT; // 启动时直接映射

    if (vmaCreateBuffer(Allocator, &bufInfo, &allocInfo, &buffer, &m_bufferAllocation, &m_bufferAllocationInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("VMA dynamic buffer failed");
    }

    // 直接拿到映射指针（VMA_ALLOCATION_CREATE_MAPPED_BIT）
    m_mappedPtr = m_bufferAllocationInfo.pMappedData;

    m_buffer = buffer;
}

VulkanDynamicBuffer::~VulkanDynamicBuffer()
{
    Destroy();
}

void* VulkanDynamicBuffer::Map(uint64 offset, uint64 size)
{
    // VMA已预映射，直接返回指针
    return static_cast<uint8_t*>(m_mappedPtr) + offset;
}

void VulkanDynamicBuffer::Unmap()
{
    // VMA预映射无需手动unmap（除非显式调用vmaUnmap）
    // 这里空实现即可，保持接口一致
}

uint64 VulkanDynamicBuffer::GetDeviceAddress() const
{
    vk::BufferDeviceAddressInfo info;
    info.buffer = m_buffer;
    return (uint64)Device.getBufferAddress(info);
}

void VulkanDynamicBuffer::Destroy()
{
    if (Device)
    {
        vmaDestroyBuffer(Allocator, m_buffer, m_bufferAllocation);
    }
}

void* VulkanDynamicBuffer::GetNativeHandle()
{
    return &m_buffer;
}
