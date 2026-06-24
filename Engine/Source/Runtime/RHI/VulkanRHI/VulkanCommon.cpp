#include "pch.h"


#define VOLK_IMPLEMENTATION // 定义 Volk 实现
#define VMA_IMPLEMENTATION
#include "VulkanCommon.h"   

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE // Vulkan C++ 动态调度器存储（全局唯一，用于自动分发所有 Vulkan 函数调用）