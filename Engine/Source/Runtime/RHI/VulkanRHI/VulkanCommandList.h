#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"


class VulkanPrimaryCommandList : public RHIPrimaryCommandList
{
public:
    VulkanPrimaryCommandList(RefPtr<RHICommandPool> commandPool);

    ~VulkanPrimaryCommandList();
public:
    virtual void Reset() override;

    virtual void Begin(RHICommandType type) override;

    virtual void End() override;

    virtual void BeginRendering(RefPtr<RHISwapchain> swapchain) override;

    virtual void EndRendering(uint32 imageIndex, RefPtr<RHISwapchain> swapchain) override;

    virtual void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) override;

    virtual void SetScissor(int32 x, int32 y, uint32 width, uint32 height) override;

    virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;

    virtual void SetCullMode(CullMode cullMode) override;

    virtual void SetFrontFace(FrontFace frontFace) override;

    virtual void BindGraphicsPipeline(RHIGraphicsPipeline* pipeline) override;

    virtual void BindVertexBuffer(RefPtr<RHIBuffer> buffer, uint32 binding, uint64 offset) override;

    virtual void BindIndexBuffer(RefPtr<RHIBuffer> buffer, uint64 offset, RHIIndexType type) override;

    virtual void PushConstant(ConstantRangeFlag rangeFlag, uint32 offset, uint32 size, const void* data) override;

    virtual void Draw(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) override;

    virtual void DrawIndexed(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, uint32 vertexOffset, uint32 firstInstance) override;

    virtual void* GetNativeHandle() override;
private:
    void _TransitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32 layerCount = 1);
private:
    vk::CommandBuffer m_commandBuffer;
    vk::Pipeline* m_pipeline;
    vk::PipelineLayout* m_pipelineLayout;
};

class VulkanSecondaryCommandList : public RHISecondaryCommandList
{
public:
    VulkanSecondaryCommandList(RefPtr<RHICommandPool> commandPool);

    ~VulkanSecondaryCommandList();
public:
    virtual void Reset() override;
private:
    vk::CommandBuffer m_commandBuffer;
    vk::CommandPool m_commandPool;
};
