#include "pch.h"
#include "ConsoleServer.h"

void ConsoleServer::Init()
{

}

void ConsoleServer::Tick()
{
}

void ConsoleServer::Exit()
{
}

void ConsoleServer::RegisterCommand(const String& cmdName, CommandFunc cmdFunc)
{
}

void ConsoleServer::UnregisterCommand(const String& cmdName)
{
}

void ConsoleServer::_RegisterDefaultCommand()
{
    m_commands["help"] = std::move([this](const String& args) 
        {
            m_console.WriteLine("=== 控制台命令帮助 ===");
            m_console.WriteLine("help - 显示帮助信息");
            m_console.WriteLine("exit - 退出控制台服务");
        });
    m_commands["exit"] = std::move([this](const String& args)
        {
            m_console.WriteLine("=== 退出控制台服务 ===");
        });
}

void ConsoleServer::_UnregisterDefaultCommand()
{
}
