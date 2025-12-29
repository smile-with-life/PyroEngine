#include "pch.h"
#include "Application.h"
#include "Engine/Engine.h"
#include "Console/Console.h"

/* static */
Application& Application::GetInstance()
{
    static Application instance;
    return instance;
}
/* member */
Application::Application()
{
    AllowServer();
}

Application::~Application()
{
    FreeServer();
}

int32 Application::Run()
{
    Console::GetInstance();
#ifdef BUILD_ENGINE
    Engine engine;

    int32 error = engine.Init();

    while (!m_isQuit)
    {
        engine.Tick();
    }

    engine.Exit();

    return error;
#endif
   
}

bool Application::IsQuit() const
{
    return m_isQuit;
}

ApplicationInfo Application::About()
{
    return ApplicationInfo();
}
