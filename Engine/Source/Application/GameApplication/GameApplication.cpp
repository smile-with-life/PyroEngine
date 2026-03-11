#include "pch.h"

#include "GameApplication.h"

#include "Runtime.h"
#include "Json/Json.h"
#include "FileSystem/FileSystem.h"
#include "Console/Console.h"
#include "Event/EventSystem.h"
#include "Window/WindowManager.h"
#include "FrameService/FrameService.h"

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
    
    GConsole->Write("Game Application Init End");
    return 0;
}

void GameApplication::Tick()
{
    // 窗口服务帧更新
    GWindowManager->Tick();
    // 帧率控制服务帧更新
    GFrameService->Tick();
}

void GameApplication::Exit()
{
    GConsole->Write("Game Application Exit Start");

    // 窗口服务退出清理
    GWindowManager->Exit();
    // 帧率控制服务退出清理
    GFrameService->Exit();
    // 取消监听事件
    GEventSystem->Unsubscribe("AppQuitEvent", MemberFuncBind(OnEvent));

    GConsole->Write("Game Application Exit End");
}

void GameApplication::OnEvent(Event& event)
{
    switch (event.GetType())
    {
    case EventType("AppQuitEvent"):
        this->Quit();
        break;
    }
}