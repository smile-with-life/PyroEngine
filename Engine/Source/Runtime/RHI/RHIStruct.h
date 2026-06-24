#pragma once

#include "Core.h"
#include "RHIType.h"
#include "Memory/RefCount.h"

struct VertexAttribute;
struct VertexInputLayout;
struct RHIGraphicsPipelineState;
struct RHISubmitInfo;

class RHIResource : public RefCount
{
public:
    RHIResource(RHIResourceUsage usage)
        : m_usage(usage)
    {

    }

    virtual ~RHIResource() = default;
protected:
    RHIResourceUsage m_usage;
};

class RHIBuffer : public RHIResource
{
public:
    RHIBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
        : RHIResource(RHIResourceUsage::Buffer)
        , m_usageFlag(usageFlag)
        , m_capacity(capacity)
    {

    }

    virtual ~RHIBuffer() = default;
public:
    virtual void* GetNativeHandle() = 0;
public:
    RHIBufferUsageFlag GetBufferUsageFlag() const
    {
        return m_usageFlag;
    }

    uint64 Capacity()
    {
        return m_capacity;
    }
protected:
    RHIBufferUsageFlag m_usageFlag = RHIBufferUsageFlag::None;

    uint64 m_capacity = 0;
};

class RHIStaticBuffer : public RHIBuffer
{
public:
    RHIStaticBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
        : RHIBuffer(usageFlag, capacity)
    {

    }

    virtual ~RHIStaticBuffer() = default;
public:
    virtual void Upload(const void* data, uint64 size, uint64 offset) = 0;

    virtual uint64 GetDeviceAddress() const = 0;

    virtual void Destroy() = 0;
};

class RHIDynamicBuffer : public RHIBuffer
{
public:
    RHIDynamicBuffer(RHIBufferUsageFlag usageFlag, uint64 capacity)
        : RHIBuffer(usageFlag, capacity)
    {

    }
    virtual ~RHIDynamicBuffer() = default;
public:
    virtual void* Map(uint64 offset = 0, uint64 size = 0) = 0;

    virtual void Unmap() = 0;

    virtual uint64 GetDeviceAddress() const = 0;

    virtual void Destroy() = 0;
};

class RHIShader : public RHIResource
{
public:
    RHIShader(ShaderUsage usage)
        : RHIResource(RHIResourceUsage::Buffer)
        , m_usage(usage)
    {

    }

    virtual ~RHIShader() = default;
public:
    virtual VertexInputLayout& GetInputLayout() = 0;
public:
    ShaderUsage GetShaderUsage() const
    {
        return m_usage;
    }
protected:
    ShaderUsage m_usage = ShaderUsage::None;
};

class RHITexture : public RHIResource
{
public:
    RHITexture()
        : RHIResource(RHIResourceUsage::Texture)
    {

    }

    virtual ~RHITexture() = default;
};

class RHITexture2D : public RHITexture
{
public:
    RHITexture2D()
        : RHITexture()
    {

    }

    virtual ~RHITexture2D() = default;
};

class RHITexture3D : public RHITexture
{
public:
    RHITexture3D()
        : RHITexture()
    {

    }

    virtual ~RHITexture3D() = default;
};

class RHIGraphicsPipeline
{
public:
    virtual void* GetNativeHandle() = 0;
};

class RHIFence : public RHIResource
{
public:
    RHIFence()
        : RHIResource(RHIResourceUsage::Buffer)
    {
    }
    virtual ~RHIFence() = default;
public:
    virtual void Wait() = 0;
    virtual void Reset() = 0;
    virtual bool IsSignaled() const = 0;

    virtual void* GetNativeHandle() = 0;

};

class RHISemaphore : public RHIResource
{
public:
    RHISemaphore()
        : RHIResource(RHIResourceUsage::Buffer)
    {
    }
    virtual ~RHISemaphore() = default;
public:
    virtual void* GetNativeHandle() = 0;
};

class RHISwapchain : public RHIResource
{
public:
    RHISwapchain()
        : RHIResource(RHIResourceUsage::Swapchain)
    {

    }

    virtual ~RHISwapchain() = default;
public:
    virtual void RecreateSwapchain() = 0;

    virtual bool Present(uint32 imageIndex, RefPtr<RHISemaphore> waitSemaphore) = 0;

    virtual uint32 AcquireNextImageIndex(RefPtr<RHISemaphore> waitSemaphore) = 0;

    virtual uint32 GetWidth() = 0;

    virtual uint32 GetHeight() = 0;
};

class RHICommandPool : public RHIResource
{
public:
    RHICommandPool()
        : RHIResource(RHIResourceUsage::Buffer)
    {
    }

    virtual ~RHICommandPool() = default;
public:
    virtual void Reset() = 0;

    virtual void* GetNativeHandle() = 0;

    virtual void Destroy() = 0;
};

class RHIPrimaryCommandList : public RHIResource
{
public:
    RHIPrimaryCommandList()
        : RHIResource(RHIResourceUsage::Buffer)
    {

    }
    virtual ~RHIPrimaryCommandList() = default;
public:
    virtual void Reset() = 0;

    virtual void Begin(RHICommandType type) = 0;

    virtual void End() = 0;

    virtual void BeginRendering(RefPtr<RHISwapchain> swapchain) = 0;

    virtual void EndRendering(uint32 imageIndex, RefPtr<RHISwapchain> swapchain) = 0;

    virtual void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) = 0;

    virtual void SetScissor(int32 x, int32 y, uint32 width, uint32 height) = 0;

    virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;

    virtual void SetCullMode(CullMode cullMode) = 0;

    virtual void SetFrontFace(FrontFace frontFace) = 0;

    virtual void BindGraphicsPipeline(RHIGraphicsPipeline* pipeline) = 0;

    virtual void BindVertexBuffer(RefPtr<RHIBuffer> buffer, uint32 binding, uint64 offset) = 0;

    virtual void BindIndexBuffer(RefPtr<RHIBuffer> buffer, uint64 offset, RHIIndexType type) = 0;

    virtual void PushConstant(ConstantRangeFlag rangeFlag, uint32 offset, uint32 size, const void* data) = 0;

    virtual void Draw(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) = 0;

    virtual void DrawIndexed(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, uint32 vertexOffset, uint32 firstInstance) = 0;

    virtual void* GetNativeHandle() = 0;
};

class RHISecondaryCommandList : public RHIResource
{
public:
    RHISecondaryCommandList()
        : RHIResource(RHIResourceUsage::Buffer)
    {

    }
    virtual ~RHISecondaryCommandList() = default;
public:
    virtual void Reset() = 0;
};

struct VertexAttribute
{
    uint32 Location;            // 着色器中的 layout(location = N)
    uint32 Binding = 0;         // 所属顶点流的绑定索引
    uint32 Offset;              // 在顶点结构中的字节偏移（自动计算）
    PixelFormat Format;         // 内部格式枚举，不直接依赖 Vulkan
    String Name;
};

struct VertexInputLayout
{
    Array<VertexAttribute> Attributes;
    bool IsValid = false;
    uint32 Stride = 0;
};

struct RHIGraphicsPipelineState
{
    Array<RefPtr<RHIShader>> Shaders;
    uint64 Stride = 0;
    PrimitiveTopology PrimitiveTopology = PrimitiveTopology::TriangleList;
    bool PrimitiveRestartEnable = false;
    PolygonMode PolygonMode = PolygonMode::Fill;
    CullMode CullMode = CullMode::Back;
    FrontFace FrontFace = FrontFace::CounterClockwise;
    float LineWidth = 1.0f;
    bool DepthClampEnable = false;
    bool RasterizerDiscardEnable = false;
    bool DepthBiasEnable = false;
    float DepthBiasConstantFactor = 0.0f;
    float DepthBiasClamp = 0.0f;
    float DepthBiasSlopeFactor = 0.0f;
    SampleCount SampleCount = SampleCount::X1;
    bool SampleShadingEnable = false;
    float MinSampleShading = 1.0f;
    bool AlphaToCoverageEnable = false;
    bool AlphaToOneEnable = false;
};

struct RHISubmitInfo
{
    RefPtr<RHIPrimaryCommandList> CommandList;
    RefPtr<RHISemaphore> ImageAvailableSemaphore;
    RefPtr<RHISemaphore> RenderFinishSemaphore;
    RefPtr<RHIFence> Fence;
};



