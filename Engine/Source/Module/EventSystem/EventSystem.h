#pragma once

#include "Core.h"
#include "Event.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Container/Map.h"

#include <functional>

/// <summary>
/// 事件系统
/// </summary>
class EventSystem
{
public:
    /// <summary>
    /// 订阅事件
    /// </summary>
    /// <param name="topics"></param>
    /// <param name="callback"></param>
    void Subscribe(const char* topic, std::function<void(Event&)> callback);
    /// <summary>
    /// 订阅事件
    /// </summary>
    /// <param name="topics"></param>
    /// <param name="callback"></param>
    void Subscribe(Array<const char*> topics, std::function<void(Event&)> callback);
    /// <summary>
    /// 取消订阅
    /// </summary>
    /// <param name="topics"></param>
    void Unsubscribe(const char* topic, const std::function<void(Event&)>& callback);
    /// <summary>
    /// 取消订阅
    /// </summary>
    /// <param name="topics"></param>
    void Unsubscribe(Array<const char*> topics, const std::function<void(Event&)>& callback);
    /// <summary>
    /// 发布事件
    /// </summary>
    /// <param name="event"></param>
    void Publish(Event& event);
public:
    /// <summary>
    /// 获取控制台单例实例
    /// </summary>
    /// <returns></returns>
    static EventSystem& GetInstance();
private:
    Map<uint64, Array<std::function<void(Event&)>>> m_listeners;
};