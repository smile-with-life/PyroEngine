#include "pch.h"
#include "ConsoleService.h"

void ConsoleServer::Init()
{
    LoadSettings();
    m_console.Show();
    m_console.SetFontSize(24);
}

void ConsoleServer::Tick()
{
    String inputText;
    while (m_console.ReadInput(inputText))
    {
        if (!inputText.IsEmpty())
        {
            m_inputBuffer = inputText;
        }
    }

    // 处理特殊键
    Console::SpecialKey key;
    while (m_console.KeyDetection(key))
    {
        switch (key)
        {
        case Console::SpecialKey::Enter:
            if (!m_inputBuffer.IsEmpty())
            {
                m_commandQueue.push(m_inputBuffer);
                m_inputBuffer.Clear();
            }
            break;
        case Console::SpecialKey::Backspace:
            if (!m_inputBuffer.IsEmpty())
            {
                m_inputBuffer.RemoveRight(1);
            }
            break;
            // 其他特殊键处理...
        default:
            break;
        }
    }

    while (!m_commandQueue.empty())
    {
        String cmd = m_commandQueue.front();
        m_commandQueue.pop();
    }
    
}

void ConsoleServer::Exit()
{
    SaveSettings();
}

void ConsoleServer::LoadSettings()
{

}

void ConsoleServer::SaveSettings()
{

}

void ConsoleServer::RegisterCommand(const String& cmdName, CommandFunc cmdFunc)
{

}

void ConsoleServer::UnregisterCommand(const String& cmdName)
{

}


