#pragma once

#include "Core.h"
#include "Container/Map.h"
#include "String/String.h"
#include "Memory/RAII.h"
#include "Window/Window.h"
#include "Event/Event.h"

/// <summary>
/// 窗口管理服务类
/// </summary>
class WindowManager
{
public:
    /// <summary>
    /// 
    /// </summary>
    ~WindowManager() = default;
public:
    /// <summary>
    /// 初始化
    /// </summary>
    void Init();
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    void Tick();
    /// <summary>
    /// 退出清理
    /// </summary>
    void Exit();
public:
    uint64 CreateWindow(const WindowProps& props = WindowProps());

    bool DestroyWindow(WindowId windowId);

    void DestroyAllWindows();

    Array<WindowId> GetAllWindowIds() const;

    ViewPtr<Window> GetWindow(WindowId windowId);

    bool IsWindowValid(WindowId windowId) const;

    uint64 GetWindowCount() const;  

    void OnEvent(Event& event);
public:
    /// <summary>
    /// 获取窗口管理器实例
    /// </summary>
    /// <returns></returns>
    static WindowManager& GetInstance();
protected:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    WindowManager() = default;
private:
    /// <summary>
    /// 延迟销毁窗口
    /// 将窗口标记为待销毁，实际销毁操作在_ProcessDelayDestroy中执行
    /// </summary>
    /// <param name="windowId"></param>
    void _DelayDestroy(WindowId windowId);
    /// <summary>
    /// 处理延迟销毁的窗口
    /// 在每帧Tick时调用，执行实际的窗口销毁操作
    /// </summary>
    void _ProcessDelayDestroy();
private:
    /// <summary>
    /// 
    /// </summary>
    Map<WindowId, SharedPtr<Window>> m_windows;
    /// <summary>
    /// 
    /// </summary>
    Array<WindowId> m_delayDestroyIds;
};
