#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanFence : public RHIFence
{
public:
    VulkanFence(bool signabled = true);
    ~VulkanFence();
public:  
    virtual void Wait() override;

    virtual void Reset() override;

    virtual bool IsSignaled() const override;

    virtual void* GetNativeHandle() override;
private:
    vk::Fence m_fence;
};
