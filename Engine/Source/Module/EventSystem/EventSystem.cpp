#include "pch.h"

#include "EventSystem.h"


EventSystem& EventSystem::GetInstance()
{
    static EventSystem instance;
    return instance;
}

void EventSystem::Subscribe(const char* topic, std::function<void(Event&)> callback)
{
    Event::Type type = EventType(topic);
    auto& callbacks = m_listeners[type];

    // 检查是否已存在相同回调
    bool exists = false;
    for (const auto& existingCallback : callbacks)
    {
        if (existingCallback.target_type() == callback.target_type() &&
            existingCallback.target<void(Event&)>() == callback.target<void(Event&)>())
        {
            exists = true;
            break;
        }
    }

    if (!exists)
    {
        callbacks.Push(callback);
    }
}

void EventSystem::Subscribe(Array<const char*> topics, std::function<void(Event&)> callback)
{
    for (const auto& topic : topics)
    {
        Event::Type type = EventType(topic);
        auto& callbacks = m_listeners[type];
        
        // 检查是否已存在相同回调
        bool exists = false;
        for (const auto& existingCallback : callbacks)
        {
            if (existingCallback.target_type() == callback.target_type() &&
                existingCallback.target<void(Event&)>() == callback.target<void(Event&)>())
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            callbacks.Push(callback);
        }
    }
}

void EventSystem::Unsubscribe(const char* topic, const std::function<void(Event&)>& callback)
{
    Event::Type type = EventType(topic);
    auto iter = m_listeners.Find(type);
    if (iter != m_listeners.end())
    {
        auto& callbacks = iter->second;

        for (int64 i = 0; i < callbacks.Size(); ++i)
        {
            if (callbacks[i].target_type() == callback.target_type() &&
                callbacks[i].target<void(Event&)>() == callback.target<void(Event&)>())
            {
                callbacks.Erase(i);
            }
        }
    }
}

void EventSystem::Unsubscribe(Array<const char*> topics, const std::function<void(Event&)>& callback)
{
    for (const auto& topic : topics)
    {
        Event::Type type = EventType(topic);
        auto iter = m_listeners.Find(type);
        if (iter != m_listeners.end())
        {
            auto& callbacks = iter->second;

            for (int64 i = 0; i < callbacks.Size(); ++i)
            {
                if (callbacks[i].target_type() == callback.target_type() &&
                    callbacks[i].target<void(Event&)>() == callback.target<void(Event&)>())
                {
                    callbacks.Erase(i);
                }
            }
        }
    }
}

void EventSystem::Publish(Event& event)
{
    if (m_listeners.Contains(event.GetType()))
    {
        auto& callbacks = m_listeners[event.GetType()];
        for (auto& callback : callbacks)
        {
            callback(event);
        }
    }
}
