#pragma once

#include "Core.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanSwapchain : public RHISwapchain
{
public:
    struct Info
    {
        vk::Extent2D ImageExtent;
        uint32 ImageCount;
        vk::SurfaceFormatKHR SurfaceFormat;
        vk::Format DepthStencilFormat;
        vk::SurfaceTransformFlagsKHR Transform;
        vk::PresentModeKHR Present;
    };
public:
    VulkanSwapchain(Window* window);

    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain& other) = delete;

    VulkanSwapchain& operator=(const VulkanSwapchain& other) = delete;

    VulkanSwapchain(VulkanSwapchain&& other) = default;

    VulkanSwapchain& operator=(VulkanSwapchain&& other) = default;
public:
    virtual void RecreateSwapchain() override;

    virtual uint32 AcquireNextImageIndex(RefPtr<RHISemaphore> waitSemaphore) override;

    virtual bool Present(uint32 imageIndex, RefPtr<RHISemaphore> waitSemaphore) override;

    virtual uint32 GetWidth() override;

    virtual uint32 GetHeight() override;
public:
    vk::ImageView& GetRelayImageView();

    vk::ImageView& GetImageView(uint32 imageIndex)
    {
        return m_imageViews[imageIndex];
    }

    vk::Image& GetRelayImage()
    {
        return m_relayImage;
    }

    vk::Image& GetImage(uint32 imageIndex)
    {
        return m_images[imageIndex];
    }

private:
    void _CreateSurface();

    void _CreateSwapchain();

    void _CreateRelayImage();

    void _DestroySwapchain();
private:
    Window* m_window;
    vk::SurfaceKHR m_surface;
    vk::SwapchainKHR m_swapchain;
    Array<vk::Image> m_images;
    Array<vk::ImageView> m_imageViews;
    VulkanSwapchain::Info m_info;
    /// <summary>
    /// 中间缓冲图像
    /// </summary>
    vk::Image m_relayImage;
    /// <summary>
    /// 中间缓冲图像视图
    /// </summary>
    vk::ImageView m_relayImageView;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocationInfo;
};