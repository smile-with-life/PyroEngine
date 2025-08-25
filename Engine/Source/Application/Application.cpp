#include "pch.h"

#include "Application.h"

int32 Application::Init()
{
    m_console = Console::Create();
    m_console->Init();
    return 0;
}

void Application::Tick()
{
    m_console->Tick();
}

void Application::Exit()
{
    m_console->Exit();
    delete m_console;
}

bool Application::IsQuit()
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
