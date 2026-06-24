#include "pch.h"

#include "SpirvReflect.h"

#include "Runtime.h"
#include "Logger/Logger.h"

SpirvReflect::SpirvReflect()
{

}

SpirvReflect::~SpirvReflect()
{
}

VertexInputLayout SpirvReflect::ReflectInputLayout(const ByteArray& source)
{
    VertexInputLayout layout;
    SpvReflectResult result = {};

    // 创建反射模块
    result = spvReflectCreateShaderModule(source.Size() * sizeof(uint32), source.Data(), &m_module);
    if (result != SPV_REFLECT_RESULT_SUCCESS)
    {
        GLog->Error("");
        return layout;
    }

    // 获取输入变量数量
    uint32 varCount = 0;
    result = spvReflectEnumerateInputVariables(&m_module, &varCount, nullptr);
    if (result != SPV_REFLECT_RESULT_SUCCESS || varCount == 0)
    {
        spvReflectDestroyShaderModule(&m_module);
        return layout;
    }

    // 获取所有输入变量
    Array<SpvReflectInterfaceVariable*> inputs(varCount);
    spvReflectEnumerateInputVariables(&m_module, &varCount, inputs.Data());
    if (result != SPV_REFLECT_RESULT_SUCCESS)
    {
        spvReflectDestroyShaderModule(&m_module);
        return layout;
    }

    // 过滤 + 排序
    Array<SpvReflectInterfaceVariable*> validInputs;
    for (auto* var : inputs)
    {
        // 跳过内置变量（gl_*）
        if (!var->name) continue;
        validInputs.Add(var);
    }

    // 按 location 升序排列
    std::sort(validInputs.begin(), validInputs.end(), [](auto* a, auto* b) { return a->location < b->location; });

    // 生成顶点属性
    uint32_t offset = 0;
    for (auto* var : validInputs)
    {
        VertexAttribute attribute;

        attribute.Location = var->location;
        attribute.Format = _ConvertFormat(var->format);
        attribute.Offset = offset;
        attribute.Name = var->name;

        layout.Attributes.Add(attribute);

        // 累加偏移（按格式字节大小）
        uint32 size = _GetPixelFormatSize(attribute.Format);
        offset += size;
    }

    layout.Stride = offset;
    layout.IsValid = true;

    // 清理
    spvReflectDestroyShaderModule(&m_module);

    return layout;
}

PixelFormat SpirvReflect::_ConvertFormat(SpvReflectFormat format) const
{
    switch (format)
    {
    case SPV_REFLECT_FORMAT_R32_SFLOAT:        return PixelFormat::R32Float;
    case SPV_REFLECT_FORMAT_R32G32_SFLOAT:     return PixelFormat::RG32Float;
    case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:  return PixelFormat::RGBA32Float;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return PixelFormat::RGBA32Float;
    case SPV_REFLECT_FORMAT_R16_SFLOAT:        return PixelFormat::R16Float;
    case SPV_REFLECT_FORMAT_R16G16_SFLOAT:     return PixelFormat::RG16Float;
    case SPV_REFLECT_FORMAT_R16G16B16A16_SFLOAT: return PixelFormat::RGBA16Float;
    case SPV_REFLECT_FORMAT_R16_UINT:          return PixelFormat::R16Uint;
    case SPV_REFLECT_FORMAT_R16_SINT:          return PixelFormat::R16Sint;
    case SPV_REFLECT_FORMAT_R16G16_UINT:       return PixelFormat::RG16Uint;
    case SPV_REFLECT_FORMAT_R16G16_SINT:       return PixelFormat::RG16Sint;
    case SPV_REFLECT_FORMAT_R32_UINT:          return PixelFormat::R32Uint;
    case SPV_REFLECT_FORMAT_R32_SINT:          return PixelFormat::R32Sint;
    case SPV_REFLECT_FORMAT_R32G32_UINT:       return PixelFormat::R32Uint;
    case SPV_REFLECT_FORMAT_R32G32_SINT:       return PixelFormat::R32Sint;
    case SPV_REFLECT_FORMAT_R32G32B32A32_UINT: return PixelFormat::R32Uint;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SINT: return PixelFormat::R32Sint;
    default:
        return PixelFormat::None;
    }
}

uint32 SpirvReflect::_GetPixelFormatSize(PixelFormat format) const
{
    switch (format)
    {
        // 8-bit 单通道
    case PixelFormat::R8Unorm:
    case PixelFormat::R8Snorm:
    case PixelFormat::R8Uint:
    case PixelFormat::R8Sint:
    case PixelFormat::R8SRGB:
        return 1;

        // 8-bit 双通道
    case PixelFormat::RG8Unorm:
    case PixelFormat::RG8Snorm:
    case PixelFormat::RG8Uint:
    case PixelFormat::RG8Sint:
    case PixelFormat::RG8SRGB:
        return 2;

        // 16-bit 单通道
    case PixelFormat::R16Float:
    case PixelFormat::R16Unorm:
    case PixelFormat::R16Snorm:
    case PixelFormat::R16Uint:
    case PixelFormat::R16Sint:
        return 2;

        // 16-bit 双通道（你之前错了，这里必须返回 4）
    case PixelFormat::RG16Float:
    case PixelFormat::RG16Uint:     // <-- 重要
    case PixelFormat::RG16Sint:     // <-- 重要
        return 4;

        // 16-bit 四通道
    case PixelFormat::RGBA16Float:
        return 8;

        // 32-bit 单通道
    case PixelFormat::R32Float:
    case PixelFormat::R32Uint:
    case PixelFormat::R32Sint:
        return 4;

        // 32-bit 双通道
    case PixelFormat::RG32Float:
        return 8;

        // 32-bit 四通道
    case PixelFormat::RGBA32Float:
        return 16;

        // 8-bit 四通道 & 打包格式（都是 32bit 总大小）
    case PixelFormat::RGBA8Unorm:
    case PixelFormat::RGBA8Snorm:
    case PixelFormat::RGBA8Uint:
    case PixelFormat::RGBA8Sint:
    case PixelFormat::RGBA8SRGB:
    case PixelFormat::BGRA8Unorm:
    case PixelFormat::BGRA8SRGB:
    case PixelFormat::RGB10A2Unorm:
    case PixelFormat::RGB10A2Uint:
    case PixelFormat::R11G11B10Float:
        return 4;

        // 不认识的格式安全兜底
    default:
        return 0;
    }
}
