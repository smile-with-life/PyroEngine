#include "pch.h"

#include "GameApplication.h"

#include "Module.h"
#include "CoreService.h"

GameApplication::GameApplication()
    : Application()
{
    AllocCoreServer();
}

GameApplication::~GameApplication()
{
    FreeCoreServer();
}

int32 GameApplication::Init()
{
    GConsole->Write("Game Application Init");
    GFrameService->Init();
    GFrameService->SetMode(FramePacingMode::Fixed);
    GFrameService->SetFixedFPS(60);
    return 0;
}

void GameApplication::Tick()
{
    GFrameService->Tick();
}

void GameApplication::Exit()
{
    GConsole->Write("Game Application Exit");
    GFrameService->Exit();
}

ApplicationInfo GameApplication::About()
{
    return ApplicationInfo();
}