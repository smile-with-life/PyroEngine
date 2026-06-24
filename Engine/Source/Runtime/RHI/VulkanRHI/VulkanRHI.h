#pragma once

#include "Core.h"
#include "Runtime.h"
#include "Container/Array.h"
#include "Memory/RAII.h"
#include "Window/Window.h"
#include "RHI/RHI.h"
#include "RHI/VulkanRHI/VulkanUtils.h"

#include "VulkanCommon.h"

class VulkanRHI : public RHI
{
public:
    /// <summary>
    /// 物理设备队列族索引信息
    /// </summary>
    struct Info
    {
        /// <summary>
        /// 图形队列
        /// </summary>
        std::optional<uint32> GraphicsFamilyIndex;
        /// <summary>
        /// 计算队列
        /// </summary>
        std::optional<uint32> ComputeFamilyIndex;
        /// <summary>
        /// 传输队列
        /// </summary>
        std::optional<uint32> TransferFamilyIndex;
        /// <summary>
        /// 中间缓冲图像格式
        /// </summary>
        vk::Format ColorFormat = vk::Format::eR16G16B16A16Sfloat;

        bool IsValidQueueInfo() const
        {
            return GraphicsFamilyIndex.has_value() && ComputeFamilyIndex.has_value() && TransferFamilyIndex.has_value();
        }
    };
public:
    virtual ~VulkanRHI();
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;
public:
    virtual RefPtr<RHISwapchain> CreateSwapchain(Window* window) override;

    virtual RefPtr<RHIShader> CreateShader(ShaderUsage usage, const ByteArray& source) override;

    virtual RefPtr<RHIStaticBuffer> CreateStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity) override;

    virtual RefPtr<RHIDynamicBuffer> CreateDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity) override;

    virtual RefPtr<RHICommandPool> CreateCommandPool(RHIQueueType type) override;

    virtual RefPtr<RHIPrimaryCommandList> CreatePrimaryCommandList(RefPtr<RHICommandPool> commandPool) override;

    virtual RefPtr<RHISecondaryCommandList> CreateSecondaryCommandList(RefPtr<RHICommandPool> commandPool) override;

    virtual RefPtr<RHIFence> CreateFence(bool signabled = true) override;

    virtual RefPtr<RHISemaphore> CreateSemaphore() override;

    virtual RHIGraphicsPipeline* CreateGraphicsPipeline(RHIGraphicsPipelineState state) override;

    virtual void Submit(RHIQueueType type, RHISubmitInfo submitInfo) override;

    virtual void WaitIdle() override;
public:
    static VulkanRHI& GetInstance();
private:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    VulkanRHI();
    /// <summary>
    /// 创建 Vulkan 实例
    /// </summary>
    void _CreateVulkanInstance();
    /// <summary>
    /// 选择物理设备
    /// </summary>
    void _SetPhyiscalDevice();
    /// <summary>
    /// 获取队列族信息
    /// </summary>
    void _QueryQueueFamilyIndexInfo();
    /// <summary>
    /// 创建逻辑设备和队列
    /// </summary>
    void _CreateDevice();
    /// <summary>
    /// 创建 VMA 分配器
    /// </summary>
    void _CreateVMAAllocator();
private:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="messageSeverity"></param>
    /// <param name="messageTypes"></param>
    /// <param name="pCallbackData"></param>
    /// <param name="pUserData"></param>
    /// <returns></returns>
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL _DebugUtilsMessengerCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        vk::DebugUtilsMessageTypeFlagsEXT messageTypes,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
public:
    /// <summary>
    /// Vulkan 实例
    /// </summary>
    vk::Instance Instance;
    /// <summary>
    /// 物理设备
    /// </summary>
    vk::PhysicalDevice PhysicalDevice;
    /// <summary>
    /// 逻辑设备
    /// </summary>
    vk::Device Device;
    /// <summary>
    /// 图形队列
    /// </summary>
    vk::Queue GraphicsQueue;
    /// <summary>
    /// 计算队列
    /// </summary>
    vk::Queue ComputeQueue;
    /// <summary>
    /// 提交队列
    /// </summary>
    vk::Queue TransferQueue;
    /// <summary>
    /// VMA 分配器
    /// </summary>
    VmaAllocator Allocator = VK_NULL_HANDLE;
    /// <summary>
    /// 队列信息
    /// </summary>
    VulkanRHI::Info Info;
private:
    /// <summary>
    /// Vulkan API 版本
    /// </summary>
    uint32 m_apiVersion = VK_API_VERSION_1_3;

    vk::DebugUtilsMessengerEXT DebugMessenger;
};

Array<const char*> GetPlatformRequiredVulkanExtensions();

vk::SurfaceKHR CreatePlatformSurface(vk::Instance instance, const Window* window);