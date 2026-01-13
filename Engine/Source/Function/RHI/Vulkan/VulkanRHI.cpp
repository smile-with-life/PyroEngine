#include "pch.h"
#include "VulkanRHI.h"

/* static */
RHI& RHI::GetInstance()
{
    static VulkanRHI instance;
    return instance;
}

/* public */
VulkanRHI::VulkanRHI()
{

}

VulkanRHI::~VulkanRHI()
{

}

void VulkanRHI::Init()
{

}

void VulkanRHI::Tick()
{

}

void VulkanRHI::Exit()
{

}

GraphicsAPI VulkanRHI::Type()
{
    return GraphicsAPI::Vulkan;
}
