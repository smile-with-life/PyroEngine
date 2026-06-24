#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanCommandPool : public RHICommandPool
{
public:
    VulkanCommandPool(RHIQueueType type);

    ~VulkanCommandPool();
public:
    virtual void* GetNativeHandle() override;

    virtual void Reset() override;

    virtual void Destroy() override;
private:
    vk::CommandPool m_commandPool;
};