#include "pch.h"

#include "VulkanFence.h"

static auto& Device = VulkanRHI::GetInstance().Device;

VulkanFence::VulkanFence(bool signabled)
    : RHIFence()
{
    vk::FenceCreateInfo createInfo;
    // 若创建时即 signaled，则 Wait() 立即返回，Reset() 后重新使用
    if (signabled) 
    {
        createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    }
    m_fence = Device.createFence(createInfo);
}

VulkanFence::~VulkanFence()
{
    if (m_fence) 
    {
        Device.destroyFence(m_fence);
    }
}

void VulkanFence::Wait()
{
    // 无限等待 fence 被触发。实际项目中可改用带超时的版本避免死锁
    vk::Result result = Device.waitForFences(1, &m_fence, VK_TRUE, UINT64_MAX);
    // 可在此断言 result == vk::Result::eSuccess
}

void VulkanFence::Reset()
{
    // 必须确保 fence 当前已触发或 GPU 已不再使用它
    Device.resetFences(1, &m_fence);
}

bool VulkanFence::IsSignaled() const
{
    // 查询状态，不会阻塞
    vk::Result status = Device.getFenceStatus(m_fence);
    return status == vk::Result::eSuccess;
}

void* VulkanFence::GetNativeHandle()
{
    return &m_fence;
}
