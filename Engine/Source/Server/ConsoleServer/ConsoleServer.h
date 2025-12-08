#pragma once
#include "Core.h"
#include "Module/Module.h"
#include "Console/Console.h"
#include "Container/Map.h"
#include "Thread/Mutex.h"


// 控制台命令
class ConsoleCommand
{
public:
    String Name;

};

class ConsoleServer : public CoreModule
{
public:
    using CommandFunc = std::function<void(const String& args)>;
public:
    virtual void Init() override;

    virtual void Tick() override;

    virtual void Exit() override;

    // 注册控制台命令
    void RegisterCommand(const String& cmdName, CommandFunc cmdFunc);

    // 注销控制台命令
    void UnregisterCommand(const String& cmdName);
private:
    /// <summary>
    /// 注册内置控制台命令
    /// </summary>
    void _RegisterDefaultCommand();

    /// <summary>
    /// 注销内置控制台命令
    /// </summary>
    void _UnregisterDefaultCommand();
private:
    /// <summary>
    /// 获取控制台单例
    /// </summary>
    Console& m_console = Console::GetInstance();

    /// <summary>
    /// 命令注册表
    /// </summary>
    Map<const String, CommandFunc> m_commands;

    /// <summary>
    /// 输入队列（存储待处理的控制台输入）
    /// </summary>
    std::queue<std::string> m_inputQueue;
};