#include "pch.h"

#include "Application.h"

#include "Module.h"

static Application* ApplicationInstance = nullptr;

/* static */
Application& Application::Create(ApplicationType type)
{
    // 清理旧实例
    if (ApplicationInstance)
    {
        delete ApplicationInstance;
        ApplicationInstance = nullptr;
    }

    // 根据类型创建对应的应用程序
    switch (type)
    {
    case ApplicationType::None:
        ApplicationInstance = new Application();
        break;
    case ApplicationType::Game:
        break;
    case ApplicationType::Editor:
        break;
    case ApplicationType::App:
        break;
    case ApplicationType::Server:
        break;
    }
    return *ApplicationInstance;
}

/* member */
Application::Application()
{
    
}

Application::~Application()
{
    
}

int32 Application::Init()
{
    GConsole->Write("Default Application Init");
    return 0;
}

void Application::Tick()
{
    GConsole->Write("Default Application Tick");
    m_isQuit = true;
}

void Application::Exit()
{
    GConsole->Write("Default Application Exit");
}

bool Application::IsQuit() const
{
    return m_isQuit;
}

ApplicationInfo Application::About()
{
    return ApplicationInfo();
}
