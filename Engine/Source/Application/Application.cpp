#include "pch.h"

#include "Application.h"

int32 Application::Init()
{
    return 0;
}

void Application::Tick()
{

}

void Application::Exit()
{

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
