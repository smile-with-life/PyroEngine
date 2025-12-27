#include "pch.h"
#include "Engine.h"
#include "Service/Service.h"

int32 Engine::Init()
{
    GFrameService->Init();
    GWindowService->Init();

    GWindowService->CreateOSWindow("GameWindow");

    return 0;
}

void Engine::Tick()
{
    GFrameService->Tick();
    GWindowService->Tick();
}

void Engine::Exit()
{
    GWindowService->DestroyOSWindow("GameWindow");

    GFrameService->Exit();
    GWindowService->Exit();
}
