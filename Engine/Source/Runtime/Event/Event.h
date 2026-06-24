#pragma once

#include "Core.h"
#include "String/String.h"

/// <summary>
/// 事件类别
/// </summary>
enum class EventCategory
{
	None,
	App,
	Window,
	Custom,
};

/// <summary>
/// 
/// </summary>
class Event
{
public:
	using Type = uint32;
public:
	Event() = default;

	virtual ~Event() = default;
public:
	virtual Type GetType() const = 0;

    virtual EventCategory GetCategory() const = 0;
public:
	// 已处理标志
	bool Handled = false;
};

/// <summary>
/// 用于将字符串转换为事件类型的函数
/// 使用FNV-1a哈希算法计算字符串的哈希值
/// </summary>
/// <param name="type">字符串</param>
/// <returns>事件类型</returns>
constexpr Event::Type EventType(StringView type)
{
	uint32 hash = 2166136261u;
	for (char c : type) {
		hash ^= static_cast<uint32_t>(c);
		hash *= 16777619u;
	}
	return hash;
}

/* ==================== AppEvent ==================== */

class AppQuitEvent : public Event
{
public:
	AppQuitEvent() = default;

	virtual ~AppQuitEvent() = default;
public:
	virtual Type GetType() const override
	{
        return EventType("AppQuitEvent");
	}

	virtual EventCategory GetCategory() const override
	{
		return EventCategory::App;
	}
};

/* ==================== WindowEvent ==================== */

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

    virtual ~WindowCloseEvent() = default;
public:
	virtual Type GetType() const override
	{
		return EventType("WindowCloseEvent");
	}

	virtual EventCategory GetCategory() const override
	{
		return EventCategory::Window;
	}
public:
    uint64 WindowId = 0;
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent() = default;

	virtual ~WindowResizeEvent() = default;
public:
	virtual Type GetType() const override
	{
		return EventType("WindowResizeEvent");
	}

	virtual EventCategory GetCategory() const override
	{
		return EventCategory::Window;
	}
public:
	uint32 Width;
	uint32 Height;
};