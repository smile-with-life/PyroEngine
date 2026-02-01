#pragma once
#include "Core.h"
#include "RHI/RHI.h"

class VulkanRHI : public RHI
{
public:
    VulkanRHI();

    virtual ~VulkanRHI();
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;

    virtual GraphicsAPI Type() override;
};