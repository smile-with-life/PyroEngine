#include "pch.h"

#include "Application.h"

int32 Application::Init()
{
    m_console = Console::Create();
    return 0;
}

void Application::Tick()
{

}

void Application::Exit()
{
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
