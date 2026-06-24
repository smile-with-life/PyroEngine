#include "pch.h"

#include "VulkanSemaphore.h"

static auto& Device = VulkanRHI::GetInstance().Device;

VulkanSemaphore::VulkanSemaphore()
    : RHISemaphore()
{
    vk::SemaphoreCreateInfo createInfo;
    m_semaphore = Device.createSemaphore(createInfo);
}

VulkanSemaphore::~VulkanSemaphore()
{
    if (m_semaphore) 
    {
        Device.destroySemaphore(m_semaphore);
    }
}

void* VulkanSemaphore::GetNativeHandle()
{
    return &m_semaphore;
}
