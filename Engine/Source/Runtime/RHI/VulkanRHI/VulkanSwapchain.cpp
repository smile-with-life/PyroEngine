#include "pch.h"

#include "VulkanSwapchain.h"

#include "Runtime.h"
#include "Container/Set.h"
#include "RHI/VulkanRHI/VulkanRHI.h"
#include "Logger/Logger.h"

static auto& Instance = VulkanRHI::GetInstance().Instance;

static auto& GraphicsQueue = VulkanRHI::GetInstance().GraphicsQueue;

static auto& Device = VulkanRHI::GetInstance().Device;

static auto& Allocator = VulkanRHI::GetInstance().Allocator;

static auto& PhysicalDevice = VulkanRHI::GetInstance().PhysicalDevice;

static auto& VulkanRHIInfo = VulkanRHI::GetInstance().Info;

VulkanSwapchain::VulkanSwapchain(Window* window)
    : m_window(window)
{
    _CreateSurface();
    _CreateSwapchain();
}

VulkanSwapchain::~VulkanSwapchain()
{
    _DestroySwapchain();

    if (m_surface)
    {
        Instance.destroySurfaceKHR(m_surface);
        m_surface = nullptr;
    }
}

void VulkanSwapchain::RecreateSwapchain()
{
    // 先释放所有旧资源（无论窗口状态如何）
    _DestroySwapchain();

    uint32 width = m_window->GetWidth();
    uint32 height = m_window->GetHeight();

    if (width == 0 || height == 0)
    {
        m_swapchain = nullptr;
        m_info.ImageExtent.width = 0;
        m_info.ImageExtent.height = 0;
        GLog->Info("Window minimized, Swapchain not recreated.");
        return;
    }

    _CreateSwapchain();
}

uint32 VulkanSwapchain::AcquireNextImageIndex(RefPtr<RHISemaphore> waitSemaphore)
{
    auto waitSemaphorePtr = static_cast<vk::Semaphore*>(waitSemaphore->GetNativeHandle());
    return Device.acquireNextImageKHR(m_swapchain, UINT64_MAX, *waitSemaphorePtr).value;
}

bool VulkanSwapchain::Present(uint32 imageIndex, RefPtr<RHISemaphore> waitSemaphore)
{
    auto waitSemaphorePtr = static_cast<vk::Semaphore*>(waitSemaphore->GetNativeHandle());

    vk::PresentInfoKHR info;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = waitSemaphorePtr;
    info.swapchainCount = 1;
    info.pSwapchains = &m_swapchain;
    info.pImageIndices = &imageIndex;

    auto result = GraphicsQueue.presentKHR(info);
    return 1;
}

uint32 VulkanSwapchain::GetWidth()
{
    return m_info.ImageExtent.width;
}

uint32 VulkanSwapchain::GetHeight()
{
    return m_info.ImageExtent.height;
}

vk::ImageView& VulkanSwapchain::GetRelayImageView()
{
    return m_relayImageView;
}

void VulkanSwapchain::_CreateSurface()
{
    m_surface = CreatePlatformSurface(Instance, m_window);

    if (!m_surface)
    {
        GLog->Error("Failed to create Vulkan Surface");
    }
}

void VulkanSwapchain::_CreateSwapchain()
{
    uint32 width = m_window->GetWidth();
    uint32 height = m_window->GetHeight();

    // 查询支持的表面格式，选择一个合适的格式
    auto formats = PhysicalDevice.getSurfaceFormatsKHR(m_surface);
    m_info.SurfaceFormat = formats[0];
    for (const auto& format : formats)
    {
        if (format.format == vk::Format::eR8G8B8A8Srgb &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            m_info.SurfaceFormat = format;
            break;
        }
    }
    // 查询支持的表面能力，设置交换链的图像数量和尺寸
    auto capabilities = PhysicalDevice.getSurfaceCapabilitiesKHR(m_surface);
    m_info.ImageCount = std::clamp<uint32>(2, capabilities.minImageCount, capabilities.maxImageCount);
    m_info.ImageExtent.width = std::clamp<uint32>(width, capabilities.minImageExtent.width, capabilities.minImageExtent.width);
    m_info.ImageExtent.height = std::clamp<uint32>(height, capabilities.minImageExtent.height, capabilities.minImageExtent.height);
    m_info.Transform = capabilities.currentTransform;
    // 查询支持的表面呈现模式，选择一个合适的模式
    auto presents = PhysicalDevice.getSurfacePresentModesKHR(m_surface);
    m_info.Present = vk::PresentModeKHR::eFifo;
    for (const auto& present : presents)
    {
        if (present == vk::PresentModeKHR::eMailbox)
        {
            m_info.Present = present;
            break;
        }
    }

    // 获取队列族索引集合
    Set<uint32> indexSet = { VulkanRHIInfo.GraphicsFamilyIndex.value(),
                             VulkanRHIInfo.ComputeFamilyIndex.value(),
                             VulkanRHIInfo.TransferFamilyIndex.value() };
    Array<uint32> includes;
    for (auto& index : indexSet)
    {
        includes.Add(index);
    }

    // 创建交换链
    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.clipped = true;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.surface = m_surface;
    createInfo.imageColorSpace = m_info.SurfaceFormat.colorSpace;
    createInfo.imageFormat = m_info.SurfaceFormat.format;
    createInfo.imageExtent = m_info.ImageExtent;
    createInfo.minImageCount = m_info.ImageCount;
    createInfo.presentMode = m_info.Present;
    createInfo.queueFamilyIndexCount = includes.Size();
    createInfo.pQueueFamilyIndices = includes.Data();
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;

    m_swapchain = Device.createSwapchainKHR(createInfo);
    m_images = Device.getSwapchainImagesKHR(m_swapchain);

    for (int32 i = 0; i < m_images.Size(); i++)
    {
        vk::ImageViewCreateInfo createInfo;
        vk::ComponentMapping mapping;
        vk::ImageSubresourceRange range;
        range.baseMipLevel = 0;
        range.levelCount = 1;
        range.baseArrayLayer = 0;
        range.layerCount = 1;
        range.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.image = m_images[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.components = mapping;
        createInfo.format = m_info.SurfaceFormat.format;
        createInfo.subresourceRange = range;

        m_imageViews.Add(Device.createImageView(createInfo));
    }

    vk::ImageCreateInfo imageInfo;
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.format = VulkanRHIInfo.ColorFormat;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = vk::SampleCountFlagBits::e1;
    imageInfo.tiling = vk::ImageTiling::eOptimal;
    imageInfo.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc;
    imageInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;  // 深度图像应该放在设备本地内存

    VkImage relayImage;
    auto ImageInfo = static_cast<VkImageCreateInfo>(imageInfo);
    vmaCreateImage(Allocator, &ImageInfo, &allocInfo, &relayImage, &m_allocation, &m_allocationInfo);

    m_relayImage = relayImage;

    vk::ImageViewCreateInfo viewInfo;
    viewInfo.image = m_relayImage;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = VulkanRHIInfo.ColorFormat;
    viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    m_relayImageView = Device.createImageView(viewInfo);

}

void VulkanSwapchain::_CreateRelayImage()
{
    
}

void VulkanSwapchain::_DestroySwapchain()
{
    Device.waitIdle();

    if (m_swapchain) 
    {
        for (auto& view : m_imageViews)
        {
            Device.destroyImageView(view);
        } 
        Device.destroySwapchainKHR(m_swapchain);
        m_swapchain = nullptr;
    }

    if (m_relayImageView) 
    {
        Device.destroyImageView(m_relayImageView);
        m_relayImageView = nullptr;
    }

    if (m_relayImage) 
    {
        vmaDestroyImage(Allocator, m_relayImage, m_allocation);
        m_relayImage = nullptr;
    }

    m_imageViews.Clear();
    m_images.Clear();
}