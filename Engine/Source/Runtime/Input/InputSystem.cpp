#include "pch.h"

#include "InputSystem.h"

/* ==================== static ==================== */
InputSystem& InputSystem::GetInstance()
{
    static InputSystem instance;
    return instance;
}

/* ==================== public ==================== */
void InputSystem::Init()
{

}

void InputSystem::Tick()
{
    for (auto& data : m_deferredInputDatas)
    {
        DispatchInput(data.first, data.second);
    }
    m_deferredInputDatas.Clear();
}

void InputSystem::Exit()
{

}

void InputSystem::AddInputHandler(uint64 id, IGenericInputHandler* handler)
{
    if (!handler) return;

    auto& inputHandlers = m_inputHandlePaths[id];
    inputHandlers.Add(handler);
    _SortByPriority(inputHandlers);
}

void InputSystem::RemoveInputHandler(uint64 id, IGenericInputHandler* handler)
{
    if (!handler) return;

    if (m_inputHandlePaths.Contains(id))
    {
        auto& inputHandlers = m_inputHandlePaths[id];
        auto iter = std::find(inputHandlers.begin(), inputHandlers.end(), handler);
        if (iter != inputHandlers.end())
        {
            inputHandlers.Erase(iter);
        }
    }
}

void InputSystem::DeferredProcessInput(uint64 id, const InputData& input)
{
    m_deferredInputDatas.Add({ id, input });
}

void InputSystem::DispatchInput(uint64 id, const InputData& input)
{
    if (m_inputHandlePaths.Contains(id))
    {
        auto& inputHandlers = m_inputHandlePaths[id];
        for (auto* handler : inputHandlers)
        {
            if (handler && handler->HandleInput(input))
            {
                break; // 被拦截，不再传递
            }
        }
    }
}

void InputSystem::ClearChannel(uint64 id)
{
    auto iter = m_inputHandlePaths.Find(id);
    if (iter != m_inputHandlePaths.end())
    {
        // 清空该通道的处理器列表
        iter->second.Clear();
    }
}

void InputSystem::ClearAll()
{
    // 清空整个哈希表
    m_inputHandlePaths.Clear();
}

bool InputSystem::Contains(uint64 id)
{
    return m_inputHandlePaths.Contains(id);
}

bool InputSystem::Contains(uint64 id, IGenericInputHandler* handler)
{
    if (!handler) return false;

    if (m_inputHandlePaths.Contains(id))
    {
        auto& handlers = m_inputHandlePaths[id];
        return std::find(handlers.begin(), handlers.end(), handler) != handlers.end();
    }

    return false;
}

/* ==================== private ==================== */
void InputSystem::_SortByPriority(Array<IGenericInputHandler*>& inputHandlers)
{
    std::sort(inputHandlers.begin(), inputHandlers.end(),
        [](IGenericInputHandler* left, IGenericInputHandler* right)
        {
            return left->GetPriority() > right->GetPriority();
        });
}