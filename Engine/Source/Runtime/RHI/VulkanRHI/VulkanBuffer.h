#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanStaticBuffer : public RHIStaticBuffer
{
public:
    VulkanStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity);

    ~VulkanStaticBuffer();
public:
    virtual void Upload(const void* data, uint64 size, uint64 offset) override;

    virtual uint64 GetDeviceAddress() const override;

    virtual void Destroy()  override;

    virtual void* GetNativeHandle() override;
private:
    VulkanRHI& m_rhi = VulkanRHI::GetInstance();
    vk::Buffer m_buffer;
    VmaAllocation m_bufferAllocation;
    VmaAllocationInfo m_bufferAllocationInfo;
};

class VulkanDynamicBuffer : public RHIDynamicBuffer
{
public:
    VulkanDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity);

    ~VulkanDynamicBuffer();
public:
    virtual void* Map(uint64 offset = 0, uint64 size = 0) override;

    virtual void Unmap() override;

    virtual uint64 GetDeviceAddress() const override;

    virtual void Destroy() override;

    virtual void* GetNativeHandle() override;
private:
    VulkanRHI& m_rhi = VulkanRHI::GetInstance();
    vk::Buffer m_buffer;
    VmaAllocation m_bufferAllocation;
    VmaAllocationInfo m_bufferAllocationInfo;
    void* m_mappedPtr = nullptr;
};