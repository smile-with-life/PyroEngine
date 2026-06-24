#include "pch.h"

#include "VulkanCommandPool.h"

static auto& Instance = VulkanRHI::GetInstance().Instance;

static auto& Device = VulkanRHI::GetInstance().Device;

static auto& PhysicalDevice = VulkanRHI::GetInstance().PhysicalDevice;

static auto& GraphicsQueue = VulkanRHI::GetInstance().GraphicsQueue;

static auto& ComputeQueue = VulkanRHI::GetInstance().ComputeQueue;

static auto& TransferQueue = VulkanRHI::GetInstance().TransferQueue;

static auto& VulkanRHIInfo = VulkanRHI::GetInstance().Info;

VulkanCommandPool::VulkanCommandPool(RHIQueueType type)
    : RHICommandPool()
{
    uint32 queueFamilyIndex = 0;    
    switch (type)   
    {
    case RHIQueueType::Graphics:
        queueFamilyIndex = VulkanRHIInfo.GraphicsFamilyIndex.value();
        break;
    case RHIQueueType::Compute:
        queueFamilyIndex = VulkanRHIInfo.ComputeFamilyIndex.value();
        break;
    case RHIQueueType::Transfer:
        queueFamilyIndex = VulkanRHIInfo.TransferFamilyIndex.value();
        break;
    case RHIQueueType::TileMapping:
        break;
    default:
        break;
    }

    vk::CommandPoolCreateInfo createInfo;
    createInfo.queueFamilyIndex = queueFamilyIndex;
    createInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    m_commandPool = Device.createCommandPool(createInfo);
}

VulkanCommandPool::~VulkanCommandPool()
{
    Destroy();
}

void* VulkanCommandPool::GetNativeHandle()
{
    return &m_commandPool;
}

void VulkanCommandPool::Reset()
{
    Device.resetCommandPool(m_commandPool, vk::CommandPoolResetFlags());
}

void VulkanCommandPool::Destroy()
{
    if(m_commandPool)
    {
        Device.destroyCommandPool(m_commandPool);
        m_commandPool = nullptr;
    }
}
