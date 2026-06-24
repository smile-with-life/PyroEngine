#include "pch.h"

#include "RHI.h"

RHI::RHI(GraphicsAPI api)
    : m_api(api)
{

}

GraphicsAPI RHI::GetGraphicsAPI() const
{
    return GraphicsAPI::None;
}

RHI& RHI::GetInstance(GraphicsAPI api)
{
    return PlatformCreateRHI(api);
}
