#include "pch.h"
#include "Engine.h"
#include "Server/Server.h"

int32 Engine::Init()
{
    GConsoleServer->Init();
    GFrameServer->Init();
    return 0;
}

void Engine::Tick()
{
    GConsoleServer->Tick();

    GFrameServer->Tick();
}

void Engine::Exit()
{
    GConsoleServer->Exit();
    GFrameServer->Exit();
}
