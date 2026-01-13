#pragma once
#include "Core.h"
#include "Service.h"
#include "RHI/RHI.h"



struct RenderConfig
{
    bool IsValidation = true;           // 是否启用API级参数校验
    bool IsDebugLayer = true;           // 是否启用驱动层调试信息输出
    bool enableGPUValidation = false;   // 是否启用GPU侧校验
};

class RenderService : public CoreService
{
public:
    virtual void Init()
    {
        m_rhi.Init();
    }

    virtual void Tick();

    virtual void Exit();

    virtual void LoadSettings();

    virtual void SaveSettings();
private:
    RHI& m_rhi = RHI::GetInstance();
};