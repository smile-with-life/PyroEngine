#include "pch.h"

#include "VulkanCommandList.h"

#include "RHI/VulkanRHI/VulkanSwapchain.h"
#include "RHI/VulkanRHI/VulkanPipeline.h"

static auto& Device = VulkanRHI::GetInstance().Device;

VulkanPrimaryCommandList::VulkanPrimaryCommandList(RefPtr<RHICommandPool> commandPool)
    : RHIPrimaryCommandList()
{
    auto poolPtr = static_cast<vk::CommandPool*>(commandPool->GetNativeHandle());

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = *poolPtr;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
        
    m_commandBuffer = VulkanRHI::GetInstance().Device.allocateCommandBuffers(allocInfo)[0];
}

VulkanPrimaryCommandList::~VulkanPrimaryCommandList()
{

}

void VulkanPrimaryCommandList::Reset()
{
    m_commandBuffer.reset();
}

void VulkanPrimaryCommandList::Begin(RHICommandType type)
{
    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.setFlags(Vulkan::CommandTypeVulkan(type));
    m_commandBuffer.begin(beginInfo);
}

void VulkanPrimaryCommandList::End()
{
    m_commandBuffer.end();
}

void VulkanPrimaryCommandList::BeginRendering(RefPtr<RHISwapchain> swapchain)
{
    auto swapchainPtr = static_cast<VulkanSwapchain*>(swapchain.RawPtr());

    // 强制转换中继图像到颜色附件布局
    _TransitionImageLayout(
        swapchainPtr->GetRelayImage(),
        vk::ImageLayout::eUndefined,               // 旧布局无所谓
        vk::ImageLayout::eColorAttachmentOptimal,
        1
    );

    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = swapchainPtr->GetRelayImageView();
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

    vk::RenderingInfo renderingInfo;
    renderingInfo.viewMask = 0;
    renderingInfo.layerCount = 1;
    renderingInfo.renderArea.offset.x = 0;
    renderingInfo.renderArea.offset.y = 0;
    renderingInfo.renderArea.extent.width = (uint32)swapchainPtr->GetWidth();
    renderingInfo.renderArea.extent.height = (uint32)swapchainPtr->GetHeight();
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    m_commandBuffer.beginRendering(renderingInfo);
}

void VulkanPrimaryCommandList::EndRendering(uint32 imageIndex, RefPtr<RHISwapchain> swapchain)
{
    m_commandBuffer.endRendering();

    auto swapchainPtr = static_cast<VulkanSwapchain*>(swapchain.RawPtr());
    auto width = swapchainPtr->GetWidth();
    auto height = swapchainPtr->GetHeight();
    auto relayImage = swapchainPtr->GetRelayImage();
    auto swapImage = swapchainPtr->GetImage(imageIndex);

    // -------------------------------------------------------------------------
    // 1. 中间缓冲 → 传输源
    // -------------------------------------------------------------------------
    _TransitionImageLayout(
        relayImage,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::ImageLayout::eTransferSrcOptimal
    );

    // -------------------------------------------------------------------------
    // 2. 交换链图像 → 传输目标
    // -------------------------------------------------------------------------
    _TransitionImageLayout(
        swapImage,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal
    );

    // -------------------------------------------------------------------------
    // 3. 🔥 Blit：浮点格式 → 交换链格式（硬件自动转换）
    // -------------------------------------------------------------------------
    vk::ImageBlit blit{};
    blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    blit.srcSubresource.layerCount = 1;
    blit.srcOffsets[1].x = width;
    blit.srcOffsets[1].y = height;
    blit.srcOffsets[1].z = 1;
    blit.dstSubresource = blit.srcSubresource;
    blit.dstOffsets[1] = blit.srcOffsets[1];

    m_commandBuffer.blitImage(
        relayImage, vk::ImageLayout::eTransferSrcOptimal,
        swapImage, vk::ImageLayout::eTransferDstOptimal,
        1, &blit,
        vk::Filter::eLinear
    );

    // -------------------------------------------------------------------------
    // 4. 交换链 → 呈现布局
    // -------------------------------------------------------------------------
    _TransitionImageLayout(
        swapImage,
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::ePresentSrcKHR
    );
}

void VulkanPrimaryCommandList::SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
{
    vk::Viewport viewport(x, y, width, height, minDepth, maxDepth);
    m_commandBuffer.setViewport(0, viewport);
    
}

void VulkanPrimaryCommandList::SetScissor(int32 x, int32 y, uint32 width, uint32 height)
{
    vk::Rect2D scissor({ x, y }, { width, height });
    m_commandBuffer.setScissor(0, scissor);
}

void VulkanPrimaryCommandList::SetPrimitiveTopology(PrimitiveTopology topology)
{
    m_commandBuffer.setPrimitiveTopology(Vulkan::PrimitiveTopologyVulkan(topology));
}

void VulkanPrimaryCommandList::SetCullMode(CullMode cullMode)
{
    m_commandBuffer.setCullMode(Vulkan::CullModeVulkan(cullMode));

}

void VulkanPrimaryCommandList::SetFrontFace(FrontFace frontFace)
{
    m_commandBuffer.setFrontFace(Vulkan::FrontFaceVulkan(frontFace));
}

void VulkanPrimaryCommandList::BindGraphicsPipeline(RHIGraphicsPipeline* pipeline)
{
    VulkanGraphicsPipeline* pipelinePtr = static_cast<VulkanGraphicsPipeline*>(pipeline);
    m_pipeline = pipelinePtr->GetPipeline();
    m_pipelineLayout = pipelinePtr->GetPipelineLayout();
    m_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
}

void VulkanPrimaryCommandList::BindVertexBuffer(RefPtr<RHIBuffer> buffer, uint32 binding, uint64 offset)
{
    vk::Buffer vkBuffer = *(vk::Buffer*)buffer->GetNativeHandle();
    m_commandBuffer.bindVertexBuffers(binding, { vkBuffer }, { offset });
}

void VulkanPrimaryCommandList::BindIndexBuffer(RefPtr<RHIBuffer> buffer, uint64 offset, RHIIndexType type)
{
    vk::Buffer vkBuffer = *(vk::Buffer*)buffer->GetNativeHandle();

    vk::IndexType vkIndexType;
    switch (type)
    {
    case RHIIndexType::Uint16: vkIndexType = vk::IndexType::eUint16; break;
    case RHIIndexType::Uint32: vkIndexType = vk::IndexType::eUint32; break;
    default: vkIndexType = vk::IndexType::eUint32; break;
    }

    m_commandBuffer.bindIndexBuffer(vkBuffer, offset, vkIndexType);
}

void VulkanPrimaryCommandList::PushConstant(ConstantRangeFlag rangeFlag, uint32 offset, uint32 size, const void* data)
{
    vk::ShaderStageFlags stageFlags = Vulkan::ConstantRangeFlagVulkan(rangeFlag);
    m_commandBuffer.pushConstants(*m_pipelineLayout, stageFlags, offset, size, data);
}

void VulkanPrimaryCommandList::Draw(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance)
{
    m_commandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanPrimaryCommandList::DrawIndexed(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, uint32 vertexOffset, uint32 firstInstance)
{
    m_commandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void* VulkanPrimaryCommandList::GetNativeHandle()
{
    return &m_commandBuffer;
}

void VulkanPrimaryCommandList::_TransitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32 layerCount)
{
    vk::ImageMemoryBarrier barrier{};
    barrier.sType = vk::StructureType::eImageMemoryBarrier;
    barrier.image = image;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = layerCount;

    vk::PipelineStageFlags srcStage, dstStage;

    if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eColorAttachmentOptimal)
    {
        barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
        dstStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    }
    else if (oldLayout == vk::ImageLayout::eColorAttachmentOptimal && newLayout == vk::ImageLayout::eTransferSrcOptimal)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
        srcStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dstStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
    {
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
        srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
        dstStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::ePresentSrcKHR)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        srcStage = vk::PipelineStageFlagBits::eTransfer;
        dstStage = vk::PipelineStageFlagBits::eBottomOfPipe;
    }

    m_commandBuffer.pipelineBarrier(srcStage, dstStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);
}

VulkanSecondaryCommandList::VulkanSecondaryCommandList(RefPtr<RHICommandPool> commandPool)
    : RHISecondaryCommandList()
{
    auto poolPtr = static_cast<vk::CommandPool*>(commandPool->GetNativeHandle());

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = *poolPtr;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = vk::CommandBufferLevel::eSecondary;

    m_commandBuffer = VulkanRHI::GetInstance().Device.allocateCommandBuffers(allocInfo)[0];
}

VulkanSecondaryCommandList::~VulkanSecondaryCommandList()
{

}

void VulkanSecondaryCommandList::Reset()
{

}

