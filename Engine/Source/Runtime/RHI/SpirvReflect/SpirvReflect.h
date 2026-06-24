#pragma once

#include "Core.h"
#include "RHI/RHI.h"

#include "spirv_reflect.h"

class SpirvReflect
{
public:
    SpirvReflect();

    ~SpirvReflect();
public:
    VertexInputLayout ReflectInputLayout(const ByteArray& source);
private:
    PixelFormat _ConvertFormat(SpvReflectFormat format) const;

    uint32 _GetPixelFormatSize(PixelFormat format) const;
private:
    SpvReflectShaderModule m_module;
};