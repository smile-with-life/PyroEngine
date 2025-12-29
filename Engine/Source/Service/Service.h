#pragma once

class Service
{
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;

    virtual bool IsSupportDynamicReload() = 0;
};

class CoreService : public Service
{
public:
    virtual void Init() = 0;

    virtual void Tick() = 0;

    virtual void Exit() = 0;

    virtual void LoadSettings()
    {

    }

    virtual void SaveSettings()
    {

    }

    virtual bool IsSupportDynamicReload() override
    {
        return false;
    }
};
