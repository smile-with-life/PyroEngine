#pragma once

#include "Core.h"
#include "Platform.h"
#include "Service.h"
#include "Module.h"
#include "Container/Map.h"
#include "String/String.h"
#include "Memory/RAII.h"
#include "Window/Window.h"


/// <summary>
/// 窗口管理服务类
/// </summary>
class WindowService : public IService
{
public:
    /// <summary>
    /// 初始化
    /// </summary>
    virtual void Init() override;
    /// <summary>
    /// 执行每一帧的更新逻辑
    /// </summary>
    virtual void Tick() override;
    /// <summary>
    /// 退出清理
    /// </summary>
    virtual void Exit() override;
    /// <summary>
    /// 是否支持动态加载
    /// </summary>
    /// <returns></returns>
    virtual bool IsSupportDynamicReload() override;
public:
    uint64 CreateMainWindow(const WindowProps& props = WindowProps());

    uint64 CreateSubWindow(const WindowProps& props = WindowProps());

    bool DestroyWindow(WindowId windowId);

    void DestroyAllWindows();

    WindowId GetMainWindowId() const;

    Array<WindowId> GetAllWindowIds() const;

    WeakPtr<Window> GetWindow(WindowId windowId);

    WeakPtr<Window> GetMainWindow();

    bool IsWindowValid(WindowId windowId) const;

    int64 GetWindowCount() const;  

    void OnEvent(Event& event);
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
    Map<WindowId, SharedPtr<Window>> m_windows;
    WindowId m_mainWindowId = 0;
    Array<WindowId> m_delayDestroyIds;
};
