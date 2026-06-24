#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanSemaphore : public RHISemaphore 
{
public:
    VulkanSemaphore();

    ~VulkanSemaphore() override;
public:
    virtual void* GetNativeHandle() override;
private:
    vk::Semaphore m_semaphore;
};