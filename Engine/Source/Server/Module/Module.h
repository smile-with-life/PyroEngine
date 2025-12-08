#pragma once
#include "Core.h"

class Module
{
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;

    virtual bool IsSupportDynamicReload() = 0;
};

class CoreModule : public Module
{
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;

    virtual bool IsSupportDynamicReload() override
    {
        return false;
    }
};