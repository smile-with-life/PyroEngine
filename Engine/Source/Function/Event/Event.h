#pragma once
#include "Core.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Container/Map.h"

/// <summary>
/// 事件类型
/// </summary>
enum class EventType
{
    None,
    Input,
    Window,
    Entity,
    Physics,
    Audio,
    UI,
    Network,
    System,
    Custom,
};

class Event
{
public:
	Event() = default;

	virtual ~Event() = default;

	virtual EventType GetType() const = 0;

	virtual const char* GetName() const = 0;

	void HandleEvent()
	{
		Handled = true;
	}
public:
    // 已处理标志
	bool Handled = false;
};

/// <summary>
/// 事件分发器
/// </summary>
class EventDispatcher
{
public:
    explicit EventDispatcher(Event& event)
        : m_event(event)
    {

    }

    template<class Type, class Func>
    bool Dispatch(const Func& func)
    {
        if (!m_event.Handled)
        {
            if (m_event.GetName() == Type::GetStaticName())
            {
                func(static_cast<Type&>(m_event));
                return true;
            }
        }
        return false;
    }

private:
    Event& m_event;
};

/// <summary>
/// 事件回调绑定宏
/// <para>仅绑定类成员函数</para>
/// </summary>
#define EventCallback(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }




class EventSystem
{
public:
    /// <summary>
    /// 订阅事件
    /// </summary>
    /// <param name="name"></param>
    /// <param name="callback"></param>
    void Subscribe(String name, std::function<void(Event&)> callback)
    {
        auto& callbacks = m_dispatchs[name];
        callbacks.Push(callback);
    }
    /// <summary>
    /// 取消订阅
    /// </summary>
    /// <param name="name"></param>
    void Unsubscribe(String name)
    {
        auto iter = m_dispatchs.Find(name);
        if (iter != m_dispatchs.end())
        {
            m_dispatchs.Erase(iter);
        }
    }
    /// <summary>
    /// 发送事件
    /// </summary>
    /// <param name="event"></param>
    void SendEvent(Event& event)
    {
        if (m_dispatchs.Contains(event.GetName()))
        {
            auto& callbacks = m_dispatchs[event.GetName()];
            for (auto& callback : callbacks)
            {
                callback(event);
            }
        } 
    }
public:
    /// <summary>
    /// 获取控制台单例实例
    /// </summary>
    /// <returns></returns>
    static EventSystem& GetInstance()
    {
        static EventSystem instance;
        return instance;
    }
private:
    Map<String, Array<std::function<void(Event&)>>> m_dispatchs;
};