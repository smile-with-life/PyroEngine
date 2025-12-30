#include "pch.h"
#include "Engine.h"
#include "GlobalService.h"


int32 Engine::Init()
{
    GFrameService->Init();
    GWindowService->Init();

    GFrameService->SetMode(FramePacingMode::Fixed);
    GFrameService->SetFixedFPS(120);
    GWindowService->CreateOSWindow("GameWindow");
    GWindowService->CreateOSWindow("EditorWindow");

    return 0;
}

void Engine::Tick()
{
    GWindowService->Tick();
    GFrameService->Tick();
}

void Engine::Exit()
{
    GFrameService->Exit();
    GWindowService->Exit();
}
