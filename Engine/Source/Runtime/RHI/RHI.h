#pragma once

#include "Core.h"
#include "Window/Window.h"
#include "Memory/RAII.h"
#include "RHIType.h"
#include "RHIStruct.h"

/// <summary>
/// 渲染硬件接口
/// </summary>
class RHI
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="api"></param>
    RHI(GraphicsAPI api);
    /// <summary>
    /// 
    /// </summary>
    virtual ~RHI() = default;
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;
public:
    virtual RefPtr<RHISwapchain> CreateSwapchain(Window* window) = 0;

    virtual RefPtr<RHIShader> CreateShader(ShaderUsage usage, const ByteArray& source) = 0;

    virtual RefPtr<RHIStaticBuffer> CreateStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity) = 0;

    virtual RefPtr<RHIDynamicBuffer> CreateDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity) = 0;

    virtual RefPtr<RHICommandPool> CreateCommandPool(RHIQueueType type) = 0;

    virtual RefPtr<RHIPrimaryCommandList> CreatePrimaryCommandList(RefPtr<RHICommandPool> commandPool) = 0;

    virtual RefPtr<RHISecondaryCommandList> CreateSecondaryCommandList(RefPtr<RHICommandPool> commandPool) = 0;

    virtual RefPtr<RHIFence> CreateFence(bool signabled = true) = 0;

    virtual RefPtr<RHISemaphore> CreateSemaphore() = 0;

    virtual RHIGraphicsPipeline* CreateGraphicsPipeline(RHIGraphicsPipelineState state) = 0;

    virtual void Submit(RHIQueueType type, RHISubmitInfo submitInfo) = 0;

    virtual void WaitIdle() = 0;
public:
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    GraphicsAPI GetGraphicsAPI() const;
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="api"></param>
    /// <returns></returns>
    static RHI& GetInstance(GraphicsAPI api);
protected:
    /// <summary>
    /// 图形后端类型
    /// </summary>
    GraphicsAPI m_api;
};

/// <summary>
/// 创建 RHI
/// </summary>
/// <param name="api"></param>
/// <returns></returns>
RHI& PlatformCreateRHI(GraphicsAPI api);