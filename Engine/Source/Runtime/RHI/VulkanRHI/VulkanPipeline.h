#pragma once

#include "Core.h"
#include "Container/Array.h"

#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanGraphicsPipeline :public RHIGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(RHIGraphicsPipelineState state);

    ~VulkanGraphicsPipeline();
public:
    vk::Pipeline* GetPipeline()
    {
        return &m_pipeline;
    }

    vk::PipelineLayout* GetPipelineLayout()
    {
        return &m_pipelineLayout;
    }
public:
    virtual void* GetNativeHandle() override;
private:
    vk::Pipeline m_pipeline;
    vk::PipelineLayout m_pipelineLayout;
};