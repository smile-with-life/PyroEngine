#pragma once
#include "Core.h"
#include "Module/Module.h"
#include "Console/Console.h"

// 控制台命令
class ConsoleCommand
{

};

class ConsoleServer : public CoreModule
{
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;

    // 注册控制台命令
    void RegisterCommand(const std::string& cmdName, ConsoleCommand cmdFunc);
    
private:
    Console& m_console = Console::GetInstance();
};