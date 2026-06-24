#pragma once

// ====================== Vulkan 核心宏定义（必须在包含 Vulkan 头文件之前定义） ======================
#define VK_NO_PROTOTYPES                // 禁用 Vulkan 官方头文件的函数原型声明，交给 Volk 管理                
#define VK_ENABLE_BETA_EXTENSIONS 0     // 关闭 Vulkan 测试版扩展

// Vulkan 平台扩展支持
#ifdef PLATFORM_WINDOWS
    #define VK_USE_PLATFORM_WIN32_KHR       // 启用 Win32 平台支持      
#elif PLATFORM_LINUX

#elif PLATFORM_ANDROID

#elif PLATFORM_IOS

#elif PLATFORM_MAC

#else
    #error "Vulkan Expand Undefinded Platform"
#endif

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1    // 让 Vulkan C++ 封装使用动态调度器（适配 Volk 动态加载，不使用静态函数）

#include <volk/volk.h>               
// Vulkan 平台扩展支持
#ifdef PLATFORM_WINDOWS
    #include <vulkan/vulkan_win32.h>         
#elif PLATFORM_LINUX

#elif PLATFORM_ANDROID

#elif PLATFORM_IOS

#elif PLATFORM_MAC

#else
    #error "Vulkan Expand Undefinded Platform"
#endif
 
#include <vulkan/vulkan.hpp>        

#define VMA_STATIC_VULKAN_FUNCTIONS 0   // 关闭 VMA 的静态 Vulkan 函数调用（适配 Volk 动态加载）
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1  // 开启 VMA 动态 Vulkan 函数调用（必须和 Volk 配合使用）

#include <vma/vk_mem_alloc.h> 