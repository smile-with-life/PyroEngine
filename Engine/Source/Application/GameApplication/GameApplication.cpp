#include "pch.h"

#include "GameApplication.h"

#include "Runtime.h"
#include "Json/Json.h"
#include "FileSystem/FileSystem.h"
#include "Console/Console.h"
#include "Event/EventSystem.h"
#include "Window/WindowManager.h"
#include "FrameService/FrameService.h"
#include "Config/ConfigManager.h"
#include "RHI/RHI.h"
#include "World/WorldModule.h"

/* ==================== public ==================== */
GameApplication::GameApplication()
    : Application()
{
    
}

GameApplication::~GameApplication()
{
    
}

int32 GameApplication::Init()
{
    GConsole->Write("Game Application Init Start");

    // 注册监听事件
    GEventSystem->Subscribe("AppQuitEvent", MemberFuncBind(GameApplication::OnEvent));
    // 窗口服务初始化
    GWindowManager->Init();
    // 帧率控制服务初始化
    GFrameService->Init();
    // RHI 初始化
    GRHI->Init();
    // 创建主窗口
    CreateMainWindow();
    // 监听窗口输入
    GInputSystem->AddInputHandler(m_mainWindowId, &m_handler);

    auto windowPtr = GWindowManager->GetWindow(m_mainWindowId);

    WorldModule::GetInstance().Init();
    
    GConsole->Write("Game Application Init End");
    return 0;
}

void GameApplication::Tick()
{
    // 窗口服务帧更新
    GWindowManager->Tick();
    // 输入系统更新
    GInputSystem->Tick();
    // RHI 帧更新
    GRHI->Tick();

    WorldModule::GetInstance().Tick();
    // 帧率控制服务帧更新
    GFrameService->Tick();
}

void GameApplication::Exit()
{
    GConsole->Write("Game Application Exit Start");

    WorldModule::GetInstance().Exit();
    // 移除输入处理器
    GInputSystem->RemoveInputHandler(m_mainWindowId, &m_handler);
    // RHI 退出清理
    GRHI->Exit();
    // 窗口服务退出清理
    GWindowManager->Exit();
    // 帧率控制服务退出清理
    GFrameService->Exit();
    // 取消监听事件
    GEventSystem->Unsubscribe("AppQuitEvent", MemberFuncBind(OnEvent));

    GConsole->Write("Game Application Exit End");
}

void GameApplication::CreateMainWindow()
{
    if (GConfigManager->Contains("Window"))
    {
        WindowProps props;
        auto config = GConfigManager->GetConfig("Window");
        config.GetValue("this->Title", props.Title);
        config.GetValue("this->Width", props.Width);
        config.GetValue("this->Height", props.Height);
        config.GetValue("this->PositionX", props.PositionX);
        config.GetValue("this->PositionY", props.PositionY);
        config.GetValue("this->Opacity", props.Opacity);
        config.GetValue("this->IsVSync", props.IsVSync);
        config.GetValue("this->IsDisplayTaskbar", props.IsDisplayTaskbar);
        config.GetValue("this->IsTopmost", props.IsTopmost);
        config.GetValue("this->IsAcceptInput", props.IsAcceptInput);
        config.GetValue("this->IsHasResizeBorder", props.IsHasResizeBorder);
        config.GetValue("this->IsSupportDragFile", props.IsSupportDragFile);
        config.GetValue("this->IsHasTitlebar", props.IsHasTitlebar);

        m_mainWindowId = GWindowManager->CreateWindow(props); 
    }
}

void GameApplication::OnEvent(Event& event)
{
    switch (event.GetType())
    {
    case EventType("AppQuitEvent"):
        OnAppQuitEvent(static_cast<AppQuitEvent&>(event));
        break;
    }
}

void GameApplication::OnAppQuitEvent(AppQuitEvent& event)
{
    this->Quit();
}
