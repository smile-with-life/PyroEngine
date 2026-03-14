#include "pch.h"

#include "Application.h"

#include "Runtime.h"
#include "GameApplication/GameApplication.h"
#include "Console/Console.h"
#include "Config/ConfigManager.h"

static Application* ApplicationInstance = nullptr;

/* static */
Application& Application::Create(Application::Type type)
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
    case Type::None :
        ApplicationInstance = new Application();
        break;
    case Type::Game :
        ApplicationInstance = new GameApplication();
        break;
    case Type::Editor :
        break;
    case Type::App :
        break;
    case Type::Server :
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
    m_isQuit = true;
}

void Application::Exit()
{
    GConsole->Write("Default Application Exit");
}

ApplicationInfo Application::About()
{
    ApplicationInfo info;
    if (GConfigManager->Contains("AppInfo"))
    {
        auto config = GConfigManager->GetConfig("AppInfo");
        config.GetValue("this->Name", m_info.Name);
        config.GetValue("this->Version", m_info.Version);
        config.GetValue("this->BuildNumber", m_info.BuildNumber);
        config.GetValue("this->Developer", m_info.Developer);
        config.GetValue("this->Copyright", m_info.Copyright);
    }

    return info;
}

void Application::Quit()
{
    m_isQuit = true;
}

bool Application::IsQuit() const
{
    return m_isQuit;
}

