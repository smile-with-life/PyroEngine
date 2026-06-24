#pragma once

#include "Core.h"
#include "Memory/ByteArray.h"
#include "Container/Array.h"
#include "RHI/VulkanRHI/VulkanRHI.h"

class VulkanShader : public RHIShader
{
public:
    VulkanShader(ShaderUsage usage, const ByteArray& source);

    ~VulkanShader();
public:
    virtual VertexInputLayout& GetInputLayout() override;
private:
    void _ReflectVertexInput(const ByteArray& spirvCode);
public:
    vk::ShaderModule ShaderModule;
    vk::PipelineShaderStageCreateInfo ShaderStage = {};
    vk::PipelineVertexInputStateCreateInfo VertexInputState = {};
private:
    VertexInputLayout m_layout;
};