#include "pch.h"

#include "Core.h"
#include "Platform.h"
#include "Runtime.h"
#include "Container/Array.h"
#include "RHI/VulkanRHI/VulkanRHI.h"
#include "Logger/Logger.h"

RHI& PlatformCreateRHI(GraphicsAPI api)
{
    switch (api)
    {
    case GraphicsAPI::Vulkan :
        return VulkanRHI::GetInstance();
    }

}

/* ==================== Vulkan ====================*/
Array<const char*> GetPlatformRequiredVulkanExtensions()
{
    return Array<const char*> {
        "VK_KHR_surface",
        "VK_KHR_win32_surface"
    };
}

vk::SurfaceKHR CreatePlatformSurface(vk::Instance instance, const Window* window)
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = GWindowsInstance;
    createInfo.hwnd = (HWND)window->GetNativeHandle();

    VkSurfaceKHR surface;
    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
    {
        GLog->Error("Windows Window Surface Create Failed!");
    }
    return surface;
}