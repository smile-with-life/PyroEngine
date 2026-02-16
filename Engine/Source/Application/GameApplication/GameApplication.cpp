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
    GConsole->Write("Game Application Init Start");

    GWindowService->CreateMainWindow();

    GFrameService->Init();
    GFrameService->SetMode(FramePacingMode::Fixed);
    GFrameService->SetFixedFPS(60);

    GConsole->Write("Game Application Init End");
    return 0;
}

void GameApplication::Tick()
{
    GWindowService->Tick();
    GFrameService->Tick();
}

void GameApplication::Exit()
{
    GConsole->Write("Game Application Exit Start");
    GWindowService->Exit();
    GFrameService->Exit();
    GConsole->Write("Game Application Exit End");
}

ApplicationInfo GameApplication::About()
{
    return ApplicationInfo();
}