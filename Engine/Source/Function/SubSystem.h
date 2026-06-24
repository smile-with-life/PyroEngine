#pragma once

#include "Core.h"
#include "Module/Module.h"

class IRenderSystem : public IModule
{
public:
    virtual ~IRenderSystem() = default;
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;

    virtual bool IsSupportDynamicReload() = 0;

    virtual void BeginFrame() = 0;

    virtual void EndFrame() = 0;

    virtual void Present() = 0;

    virtual void SubmitSceneData() = 0;

    virtual void SubmitUIData() = 0;
};