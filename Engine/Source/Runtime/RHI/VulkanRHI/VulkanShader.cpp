#include "pch.h"

#include "VulkanShader.h"

#include "Container/Array.h"
#include "Container/HashMap.h"
#include "RHI/VulkanRHI/VulkanRHI.h"
#include "RHI/SpirvReflect/SpirvReflect.h"

static auto& Device = VulkanRHI::GetInstance().Device;

VulkanShader::VulkanShader(ShaderUsage usage, const ByteArray& source)
    : RHIShader(usage)
{
    vk::ShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType = vk::StructureType::eShaderModuleCreateInfo;
    shaderCreateInfo.codeSize = source.Size();
    shaderCreateInfo.pCode = (const uint32*)source.Data();
    ShaderModule = Device.createShaderModule(shaderCreateInfo);

    ShaderStage.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
    ShaderStage.stage = Vulkan::ShaderTypeVulkan(m_usage);
    ShaderStage.module = ShaderModule;
    ShaderStage.pName = "main";

    // 仅顶点着色器需要反射顶点输入
    if (usage == ShaderUsage::Vertex)
    {
        SpirvReflect reflect;
        m_layout = reflect.ReflectInputLayout(source);
    }
}

VulkanShader::~VulkanShader()
{
    Device.destroyShaderModule(ShaderModule);
}

VertexInputLayout& VulkanShader::GetInputLayout()
{
    return m_layout;
}


