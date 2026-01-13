#pragma once
#include "Core.h"

enum class GraphicsAPI
{
    None,
    Vulkan,
    DirectX12,
    Metal
};

class RHI
{
public:
    RHI() = default;

    virtual ~RHI() = default;
public:
    virtual void Init();

    virtual void Tick();

    virtual void Exit();

    virtual GraphicsAPI Type();
public:
    static RHI& GetInstance();
};