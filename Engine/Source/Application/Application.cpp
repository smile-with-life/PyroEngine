#include "pch.h"

#include "Application.h"

Application& Application::GetInstance()
{
    static Application instance;
    return instance;
}

int32 Application::Init()
{
    GConsoleServer->Init();
    return 0;
    
}

void Application::Tick()
{
    GConsoleServer->Tick();
}

void Application::Exit()
{
    GConsoleServer->Exit();
}

bool Application::IsQuit() const
{
    return m_isQuit;
}

int32 Application::Run()
{
    int32 error = Init();

    while (!IsQuit())
    {
        Tick();
    }

    Exit();

    return error;
}

ApplicationInfo Application::About()
{
    return ApplicationInfo();
}
