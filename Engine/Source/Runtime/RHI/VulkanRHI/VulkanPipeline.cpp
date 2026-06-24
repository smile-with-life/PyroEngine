#include "pch.h"

#include "VulkanPipeline.h"

#include "Runtime.h"
#include "RHI/VulkanRHI/VulkanShader.h"
#include "Logger/Logger.h"

static auto& Device = VulkanRHI::GetInstance().Device;

static auto& VulkanRHIInfo = VulkanRHI::GetInstance().Info;

VulkanGraphicsPipeline::VulkanGraphicsPipeline(RHIGraphicsPipelineState state)
{
    VertexInputLayout inputLayout;
    // 着色器
    Array<vk::PipelineShaderStageCreateInfo> stages;
    for (auto& shader : state.Shaders)
    {
        auto vulkanShader = static_cast<VulkanShader*>(shader.RawPtr());
        stages.Add(vulkanShader->ShaderStage);

        if (vulkanShader->GetShaderUsage() == ShaderUsage::Vertex)
        {
            inputLayout = vulkanShader->GetInputLayout();
        }
    } 

    // 顶点输入
    Array<vk::VertexInputBindingDescription> bindings;
    Array<vk::VertexInputAttributeDescription> attributes;
    vk::PipelineVertexInputStateCreateInfo inputState = {};
    if (inputLayout.IsValid && !inputLayout.Attributes.IsEmpty())
    {
        // 创建绑定描述 (默认单个绑定 0)
        vk::VertexInputBindingDescription binding{};
        binding.binding = 0;
        if (state.Stride == inputLayout.Stride)
        {
            binding.stride = inputLayout.Stride;
        }
        else
        {
            GLog->Warning("GPU Stride Not Equal!\n\tShader Reflect Stride: {}\n\tPipeline Stride: {}", inputLayout.Stride, state.Stride);
            binding.stride = state.Stride;
        }
        
        binding.inputRate = vk::VertexInputRate::eVertex;
        bindings.Add(binding);

        // 创建属性描述
        for (const auto& attr : inputLayout.Attributes)
        {
            vk::VertexInputAttributeDescription vkAttr{};
            vkAttr.location = attr.Location;
            vkAttr.binding = attr.Binding;
            vkAttr.format = Vulkan::PixelFormatVulkan(attr.Format);
            vkAttr.offset = attr.Offset;

            attributes.Add(vkAttr);
        }

        inputState.vertexBindingDescriptionCount = (uint32)bindings.Size();
        inputState.pVertexBindingDescriptions = bindings.Data();
        inputState.vertexAttributeDescriptionCount = (uint32)attributes.Size();
        inputState.pVertexAttributeDescriptions = attributes.Data();
    }

    // 输入装配
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
    inputAssemblyState.primitiveRestartEnable = state.PrimitiveRestartEnable;
    inputAssemblyState.topology = Vulkan::PrimitiveTopologyVulkan(state.PrimitiveTopology);

    // 视口和裁剪（动态设置）
    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr;
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;

    // 光栅化
    vk::PipelineRasterizationStateCreateInfo rasterInfo;
    rasterInfo.rasterizerDiscardEnable = state.RasterizerDiscardEnable;
    rasterInfo.cullMode = Vulkan::CullModeVulkan(state.CullMode);
    rasterInfo.frontFace = Vulkan::FrontFaceVulkan(state.FrontFace);
    rasterInfo.polygonMode = Vulkan::PolygonModeVulkan(state.PolygonMode);
    rasterInfo.lineWidth = state.LineWidth;
    rasterInfo.depthClampEnable = state.DepthClampEnable;
    rasterInfo.depthBiasEnable = state.DepthBiasEnable;
    rasterInfo.depthBiasConstantFactor = state.DepthBiasConstantFactor;
    rasterInfo.depthBiasClamp = state.DepthBiasClamp;
    rasterInfo.depthBiasSlopeFactor = state.DepthBiasSlopeFactor;

    // 多重采样
    vk::PipelineMultisampleStateCreateInfo multisample;
    multisample.rasterizationSamples = Vulkan::SampleCountVulkan(state.SampleCount);
    multisample.sampleShadingEnable = state.SampleShadingEnable;
    multisample.minSampleShading = state.MinSampleShading;
    multisample.pSampleMask = nullptr;
    multisample.alphaToCoverageEnable = state.AlphaToCoverageEnable;
    multisample.alphaToOneEnable = state.AlphaToOneEnable;
    

    // 深度模板（启用深度测试）
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = false;
    depthStencil.depthWriteEnable = false;
    depthStencil.depthCompareOp = vk::CompareOp::eLess;
    depthStencil.depthBoundsTestEnable = false;
    depthStencil.stencilTestEnable = false;
    depthStencil.front;
    depthStencil.back;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;

    // 颜色混合
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.blendEnable = false;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = false;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants = vk::ArrayWrapper1D<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f });


    // 动态状态（视口和裁剪矩形）
    std::array<vk::DynamicState, 5> dynamicList = { 
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
        vk::DynamicState::ePrimitiveTopology,
        vk::DynamicState::eCullMode,
        vk::DynamicState::eFrontFace
    };
    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.dynamicStateCount = dynamicList.size();
    dynamicState.pDynamicStates = dynamicList.data();

    vk::PushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eAll;
    pushConstantRange.offset = 0;
    pushConstantRange.size = 128;

    // 管线布局
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    m_pipelineLayout = Device.createPipelineLayout(pipelineLayoutInfo);

    // 动态渲染专用信息：指定颜色附件格式
    vk::PipelineRenderingCreateInfo renderingInfo;
    std::array<vk::Format, 1> colorFormats = { VulkanRHIInfo.ColorFormat };
    renderingInfo.viewMask = 0;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachmentFormats = colorFormats.data();
    renderingInfo.depthAttachmentFormat = vk::Format::eUndefined;
    renderingInfo.stencilAttachmentFormat = vk::Format::eUndefined;

    vk::GraphicsPipelineCreateInfo createInfo;
    createInfo.stageCount= stages.Size();
    createInfo.pStages = stages.Data();
    createInfo.pVertexInputState = &inputState;
    createInfo.pInputAssemblyState = &inputAssemblyState;
    createInfo.pTessellationState = nullptr;
    createInfo.pViewportState = &viewportState;
    createInfo.pRasterizationState = &rasterInfo;
    createInfo.pMultisampleState = &multisample;
    createInfo.pDepthStencilState = &depthStencil;
    createInfo.pColorBlendState = &colorBlending;
    createInfo.pDynamicState = &dynamicState;
    createInfo.layout = m_pipelineLayout;
    createInfo.renderPass = nullptr;
    createInfo.subpass = 0;
    createInfo.basePipelineHandle = nullptr;
    createInfo.basePipelineIndex = -1;
    createInfo.pNext = &renderingInfo;

    auto result = Device.createGraphicsPipeline(nullptr, createInfo);
    if (result.result != vk::Result::eSuccess)
    {
        /*GLog->Error("Create Graphics Pipeline Failed!");*/
    }
    m_pipeline = std::move(result.value);
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
    if (m_pipeline)
    {
        Device.destroyPipeline(m_pipeline);
    }
    if (m_pipelineLayout)
    {
        Device.destroyPipelineLayout(m_pipelineLayout);
    }
}

void* VulkanGraphicsPipeline::GetNativeHandle()
{
    return &m_pipeline;
}
