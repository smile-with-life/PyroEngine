#include "pch.h"

#include "VulkanRHI.h"

#include "Container/Set.h"
#include "Console/Console.h"
#include "Logger/Logger.h"
#include "RHI/VulkanRHI/VulkanUtils.h"
#include "RHI/VulkanRHI/VulkanSwapchain.h"
#include "RHI/VulkanRHI/VulkanShader.h"
#include "RHI/VulkanRHI/VulkanBuffer.h"
#include "RHI/VulkanRHI/VulkanCommandPool.h"
#include "RHI/VulkanRHI/VulkanCommandList.h"
#include "RHI/VulkanRHI/VulkanPipeline.h"
#include "RHI/VulkanRHI/VulkanFence.h"
#include "RHI/VulkanRHI/VulkanSemaphore.h"

/* ==================== static ==================== */
VulkanRHI& VulkanRHI::GetInstance()
{
    static VulkanRHI instance;
    return instance;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL VulkanRHI::_DebugUtilsMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageTypes, const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    // 添加消息类型信息
    std::string type;
    if (messageTypes & vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral)
    {
        type += "[General]";
    }
    if (messageTypes & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
    {
        type += "[Validation]";
    }
    if (messageTypes & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
    {
        type += "[Performance]";
    }

    LogLevel level;
    if (messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
    {
        level = LogLevel::Error;
        GLog->Error("[Vulkan Error] {} : [{}]", type, pCallbackData->pMessage);
    }
    if (messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
    {
        level = LogLevel::Warning;
        GLog->Warning("[Vulkan Warning] {} : [{}]", type, pCallbackData->pMessage);
    }

    // 如果有对象标签，也输出它们
    if (pCallbackData->objectCount > 0)
    {
        for (uint32 i = 0; i < pCallbackData->objectCount; ++i)
        {
            const auto& obj = pCallbackData->pObjects[i];
            if (obj.pObjectName) {
                GLog->Log(level, "Object {}: {} ({})", i, obj.pObjectName, (uint64_t)obj.objectHandle);
            }
        }
    }

    return VK_FALSE;
}

/* ==================== public ==================== */
VulkanRHI::VulkanRHI()
    : RHI(GraphicsAPI::Vulkan)
{
    
}


VulkanRHI::~VulkanRHI()
{

}

void VulkanRHI::Init()
{
    try
    {
        // 创建 Vulkan 实例
        _CreateVulkanInstance();
        // 选择物理设备
        _SetPhyiscalDevice();
        // 获取队列族信息
        _QueryQueueFamilyIndexInfo();
        // 创建逻辑设备和队列
        _CreateDevice();
        // 创建 VMA 分配器
        _CreateVMAAllocator();
    }
    catch (...)
    {
        Exit();

    }
    
}

void VulkanRHI::Tick()
{

}

void VulkanRHI::Exit()
{
    if constexpr (GIsDebugMode)
    {
        if (DebugMessenger && Instance)
        {
            Instance.destroyDebugUtilsMessengerEXT(DebugMessenger);
        }
    }
    if (Allocator) vmaDestroyAllocator(Allocator);
    if (Device) Device.destroy();
    if (Instance) Instance.destroy();   
}

RefPtr<RHISwapchain> VulkanRHI::CreateSwapchain(Window* window)
{
    return RefPtr<RHISwapchain>(new VulkanSwapchain(window));
}

RefPtr<RHIShader> VulkanRHI::CreateShader(ShaderUsage usage, const ByteArray& source)
{
    return RefPtr<RHIShader>(new VulkanShader(usage, source));
}

RefPtr<RHIStaticBuffer> VulkanRHI::CreateStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
{
    return RefPtr<RHIStaticBuffer>(new VulkanStaticBuffer(usageFlag, capacity));
}

RefPtr<RHIDynamicBuffer> VulkanRHI::CreateDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
{
    return RefPtr<RHIDynamicBuffer>(new VulkanDynamicBuffer(usageFlag, capacity));
}

RefPtr<RHICommandPool> VulkanRHI::CreateCommandPool(RHIQueueType type)
{
    return RefPtr<RHICommandPool>(new VulkanCommandPool(type));
}

RefPtr<RHIPrimaryCommandList> VulkanRHI::CreatePrimaryCommandList(RefPtr<RHICommandPool> commandPool)
{
    return RefPtr<RHIPrimaryCommandList>(new VulkanPrimaryCommandList(commandPool));
}

RefPtr<RHISecondaryCommandList> VulkanRHI::CreateSecondaryCommandList(RefPtr<RHICommandPool> commandPool)
{
    return RefPtr<RHISecondaryCommandList>(nullptr);
}

RefPtr<RHIFence> VulkanRHI::CreateFence(bool signabled)
{
    return RefPtr<RHIFence>(new VulkanFence(signabled));
}

RefPtr<RHISemaphore> VulkanRHI::CreateSemaphore()
{
    return RefPtr<RHISemaphore>(new VulkanSemaphore());
}


RHIGraphicsPipeline* VulkanRHI::CreateGraphicsPipeline(RHIGraphicsPipelineState state)
{
    return new VulkanGraphicsPipeline(state);
}

void VulkanRHI::Submit(RHIQueueType type, RHISubmitInfo info)
{
    auto fencePtr = static_cast<vk::Fence*>(info.Fence->GetNativeHandle());
    auto commandListPtr = static_cast<vk::CommandBuffer*>(info.CommandList->GetNativeHandle());
    auto imageAvailableSemaphorePtr = static_cast<vk::Semaphore*>(info.ImageAvailableSemaphore->GetNativeHandle());
    auto renderFinishSemaphorePtr = static_cast<vk::Semaphore*>(info.RenderFinishSemaphore->GetNativeHandle());
    // 提交命令缓冲（使用同步2 API）
    vk::CommandBufferSubmitInfo cmdInfo(*commandListPtr);
    vk::SemaphoreSubmitInfo waitInfo(*imageAvailableSemaphorePtr, 0, vk::PipelineStageFlagBits2::eColorAttachmentOutput);
    vk::SemaphoreSubmitInfo signalInfo(*renderFinishSemaphorePtr, 0, vk::PipelineStageFlagBits2::eAllGraphics);
    vk::SubmitInfo2 submitInfo({}, 1, &waitInfo, 1, &cmdInfo, 1, &signalInfo);

    switch (type)
    {
    case RHIQueueType::Graphics:
        GraphicsQueue.submit2(submitInfo, *fencePtr);
        break;
    case RHIQueueType::Compute:
        ComputeQueue.submit2(submitInfo, *fencePtr);
        break;
    case RHIQueueType::Transfer:
        TransferQueue.submit2(submitInfo, *fencePtr);
        break;
    case RHIQueueType::TileMapping:
        TransferQueue.submit2(submitInfo, *fencePtr);
        break;
    }
}

void VulkanRHI::WaitIdle()
{
    Device.waitIdle();
}

/* ==================== private ==================== */
void VulkanRHI::_CreateVulkanInstance()
{
    // 初始化 Volk
    if (volkInitialize() != VK_SUCCESS) {
        GLog->Error("Failed to initialize volk!");
        return;
    }
    // 初始化 Vulkan-Hpp 默认调度器的全局函数
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    // 枚举 Vulkan 支持的层和扩展
    auto vulkanLayers = vk::enumerateInstanceLayerProperties();
    auto vulkanExtensions = vk::enumerateInstanceExtensionProperties();
    if constexpr (GIsDebugMode) // 调试模式下执行
    {
        // 输出 Vulkan 支持的层
        GConsole->Log("Vulkan support Layers:");
        for (const auto& layer : vulkanLayers)
        {
            GConsole->Log(" Vulkan Layer:[{}]", layer.layerName.data());
        }
        // 输出 Vulkan 支持的扩展
        GConsole->Log("Vulkan support Extensions:");
        for (const auto& extension : vulkanExtensions)
        {
            GConsole->Log(" Vulkan Extension:[{}]", extension.extensionName.data());
        }
    }

    // 设置启用层
    Array<const char*> layers;
    if constexpr (GIsDebugMode) // 调试模式下执行
    {
        layers.Add("VK_LAYER_KHRONOS_validation"); // 添加验证层
    }
    // 判断需要启用的层是否存在
    for (auto layerName : layers)
    {
        bool found = false;
        for (const auto& layer : vulkanLayers)
        {
            if (strcmp(layerName, layer.layerName) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            GLog->Warning("Vulkan Layer [{}] is not available", layerName);
        }
    }   

    // 设置扩展
    Array<const char*> extensions = GetPlatformRequiredVulkanExtensions();
    if constexpr (GIsDebugMode)
    {
        extensions.Add("VK_EXT_debug_utils"); // 添加调试扩展
    }
    // 判断需要的扩展是否存在
    for (auto extensionName : extensions)
    {
        bool found = false;
        for (const auto& extension : vulkanExtensions) {
            if (strcmp(extensionName, extension.extensionName) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            GLog->Warning("Vulkan Extension [{}] is not available", extensionName);
        }
    }

    // 应用信息
    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName = "PyroEngineRHI"; // 设置应用名称
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0); // 设置应用版本
    appInfo.pEngineName = "PyroEngine"; // 设置引擎名称
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0); // 设置引擎版本
    appInfo.apiVersion = m_apiVersion; // 设置 Vulkan 版本

    // 实例创建信息
    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = layers.Size();
    createInfo.ppEnabledLayerNames = layers.Data();
    createInfo.enabledExtensionCount = extensions.Size();
    createInfo.ppEnabledExtensionNames = extensions.Data();

    // 设置调试 messenger 创建信息（用于实例创建期间的调试）
    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if constexpr (GIsDebugMode)
    {
        debugCreateInfo.messageSeverity =
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;

        debugCreateInfo.messageType =
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

        debugCreateInfo.pfnUserCallback = _DebugUtilsMessengerCallback;
        debugCreateInfo.pUserData = this;

        // 将调试信息附加到 pNext 链
        createInfo.pNext = &debugCreateInfo;
    }

    try 
    {
        Instance = vk::createInstance(createInfo);

        // Volk 加载 + Vulkan-Hpp 调度
        volkLoadInstance(Instance);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(Instance);

        if constexpr (GIsDebugMode)
        {
            DebugMessenger = Instance.createDebugUtilsMessengerEXT(debugCreateInfo);
        }
    }
    catch (const vk::SystemError& err)
    {
        GLog->Error("Vulkan Instance Create Failed:[{}]", err.what());
        throw;
    }
}

void VulkanRHI::_SetPhyiscalDevice()
{
    // 枚举可用物理设备
    auto devices = Instance.enumeratePhysicalDevices();
    // 判断设备是否为空
    if (devices.empty())
    {
        GLog->Error("No Vulkan physical devices found!");
        return;
    }
    // 设备评分列表
    Array<int32> scores(devices.size());
    // 遍历物理设备列表
    for (int32 i = 0; i < devices.size(); i++)
    {   
        auto properties = devices[i].getProperties();
        auto memory = devices[i].getMemoryProperties();
        auto features = devices[i].getFeatures();
        auto queueFamilies = devices[i].getQueueFamilyProperties();

        // 设备类型评分
        switch (properties.deviceType)
        {
        case vk::PhysicalDeviceType::eDiscreteGpu:
            scores[i] += 10000;
            break;
        case vk::PhysicalDeviceType::eIntegratedGpu:
            scores[i] += 5000;
            break;
        case vk::PhysicalDeviceType::eVirtualGpu:
            scores[i] += 2000;
            break;
        case vk::PhysicalDeviceType::eCpu:
            scores[i] += 100;
            break;
        case vk::PhysicalDeviceType::eOther:
            scores[i] = -1;
            continue;
        }

        // Vulkan 版本评分
        if (properties.apiVersion >= VK_API_VERSION_1_3) 
        {
            scores[i] += 3000;
        }
        else if (properties.apiVersion >= VK_API_VERSION_1_2) 
        {
            scores[i] += 2000;
        }
        else if (properties.apiVersion >= VK_API_VERSION_1_1)
        {
            scores[i] += 1000;
        }

        // 显存容量评分（每 GB 显存加 100 分）
        vk::DeviceSize totalGPUMemory = 0;
        for (uint32_t j = 0; j < memory.memoryHeapCount; ++j) 
        {
            if (memory.memoryHeaps[j].flags & vk::MemoryHeapFlagBits::eDeviceLocal) 
            {
                totalGPUMemory += memory.memoryHeaps[j].size;
            }
        }
        scores[i] += (uint32)(totalGPUMemory / (1024 * 1024 * 1024)) * 100;

        // 必须的特性检查（几何着色器、曲面积分、各向异性过滤）
        if (!features.geometryShader ||
            !features.tessellationShader ||
            !features.samplerAnisotropy)
        {
            scores[i] -= 10000; // 
        }

        // 检查队列族支持
        bool hasGraphicsQueue = false;
        bool hasComputeQueue = false;
        bool hasTransferQueue = false;
        for (const auto& queueFamily : queueFamilies) 
        {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) 
            {
                hasGraphicsQueue = true;
            }
            if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) 
            {
                hasComputeQueue = true;
            }
            if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) 
            {
                hasTransferQueue = true;
            }
        }
        if (!hasGraphicsQueue) scores[i] -= 10000;  // 必须要有图形队列
        if (!hasComputeQueue) scores[i] -= 2000;    // 最好有计算队列
        if (!hasTransferQueue) scores[i] -= 1000;   // 最好有传输队列

        if constexpr (GIsDebugMode) // 调试模式下执行
        {
            // 输出 GPU 信息
            GConsole->Log("GPU Name:[{}] / Vulkan API:[{}] / Device Type:[{}]",
                properties.deviceName.data(),
                Vulkan::APIVersion(properties.apiVersion),
                Vulkan::DeviceType(properties.deviceType));
        }
    
    }
    
    // 挑选最佳物理设备
    int32 index = -1;
    int32 score = -1;
    for (int32 i = 0; i < scores.Size(); ++i)
    {
        if (scores[i] > score)
        {
            score = scores[i];
            index = i;
        }
    }

    if (index >= 0)
    {
        PhysicalDevice = devices[index];

        if constexpr (GIsDebugMode) // 调试模式下执行
        {
            // 输出选中的设备
            auto properties = PhysicalDevice.getProperties();
            GConsole->Log("Selected GPU [{}]: {} with score {}",
                index, properties.deviceName.data(), score);
        }
    }
    else
    {
        // 如果没有找到合适的设备，回退到第一个
        PhysicalDevice = devices[0];
        GLog->Warning("No ideal GPU found, falling back to first device");
    }  
}

void VulkanRHI::_QueryQueueFamilyIndexInfo()
{
    auto properties = PhysicalDevice.getQueueFamilyProperties();

    for (uint32 i = 0; i < properties.size(); ++i)
    {
        // 图形队列优先，找到后继续寻找计算和传输队列
        const auto& property = properties[i];
        if (property.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            Info.GraphicsFamilyIndex = i;
            continue; 
        }
        if (property.queueFlags & vk::QueueFlagBits::eCompute)
        {
            Info.ComputeFamilyIndex = i;
            continue;
        }
        if (property.queueFlags & vk::QueueFlagBits::eTransfer)
        {
            Info.TransferFamilyIndex = i;
            continue;
        }

        if (Info.IsValidQueueInfo())
        {
            break; // 已找到所有需要的队列族，退出循环
        }
    }
}

void VulkanRHI::_CreateDevice()
{
    // 需要启用的设备扩展列表
    Array<const char*> deviceExtensions = {
        "VK_KHR_swapchain"
    };

    // 准备设备命令队列创建信息
    Array<vk::DeviceQueueCreateInfo> queueCreateInfos;
    Set<uint32> queueFamilies = { Info.GraphicsFamilyIndex.value(),
                                    Info.ComputeFamilyIndex.value(),
                                    Info.TransferFamilyIndex.value() };
    float priority = 1.0;
    for (uint32 queueFamilyIndex : queueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.Add(queueCreateInfo);
    }

    // 配置设备特性
    vk::PhysicalDeviceFeatures features = {};
    features.samplerAnisotropy = true; // 启用各向异性过滤，提高纹理在倾斜角度的渲染质量
    features.fragmentStoresAndAtomics = true; // 允许片段着色器执行存储和原子操作
    features.independentBlend = true; // 启用独立的颜色附件混合状态
    features.geometryShader = true; // 启用几何着色器支持

    // 配置设备特性（Vulkan 1.2）
    vk::PhysicalDeviceVulkan12Features features_1_2;
    features_1_2.descriptorIndexing = true; // 启用描述符索引扩展
    features_1_2.shaderSampledImageArrayNonUniformIndexing = true; // 允许在着色器中非统一地索引采样图像数组
    features_1_2.descriptorBindingVariableDescriptorCount = true; // 支持变长描述符绑定的数量
    features_1_2.runtimeDescriptorArray = true; // 启用运行时描述符数组
    features_1_2.bufferDeviceAddress = true; // 允许着色器直接访问缓冲区设备地址
    features_1_2.pNext = nullptr;

    // 配置设备特性（Vulkan 1.3）
    vk::PhysicalDeviceVulkan13Features features_1_3;
    features_1_3.dynamicRendering = true; // 启用动态渲染，替代传统的渲染通道/帧缓冲对象
    features_1_3.synchronization2 = true; // 启用增强的同步原语
    features_1_3.pNext = &features_1_2; // 将 Vulkan 1.2 特性链到 Vulkan 1.3 特性之后
    
    // 创建设备
    vk::DeviceCreateInfo createInfo;
    createInfo.queueCreateInfoCount = queueCreateInfos.Size();
    createInfo.pQueueCreateInfos = queueCreateInfos.Data();
    createInfo.pEnabledFeatures = &features;
    createInfo.enabledExtensionCount = deviceExtensions.Size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.Data();
    createInfo.pNext = &features_1_3;

    try
    {
        Device = PhysicalDevice.createDevice(createInfo);

        // ======================
        // 加载设备函数（必须）
        // ======================
        volkLoadDevice(Device);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(Device);
    }
    catch (const vk::SystemError& err)
    {
        GLog->Error("Vulkan Device Create Failed:[{}]", err.what());
    }

    // 获取图形队列
    GraphicsQueue = Device.getQueue(Info.GraphicsFamilyIndex.value(), 0);
    // 获取计算队列
    ComputeQueue = Device.getQueue(Info.ComputeFamilyIndex.value(), 0);
    // 获取传输队列
    TransferQueue = Device.getQueue(Info.TransferFamilyIndex.value(), 0);
}

void VulkanRHI::_CreateVMAAllocator()
{
    // 只设置必需的两个函数
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.pVulkanFunctions = &vulkanFunctions;
    allocatorInfo.physicalDevice = PhysicalDevice;
    allocatorInfo.device = Device;
    allocatorInfo.instance = Instance;
    allocatorInfo.vulkanApiVersion = m_apiVersion;

    // 启用详细统计
    allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;

    VkResult result = vmaCreateAllocator(&allocatorInfo, &Allocator);
    if (result != VK_SUCCESS)
    {
        GLog->Error("VMA allocator creation failed with error: {}", static_cast<int>(result));
    }
}


