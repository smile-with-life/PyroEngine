#pragma once

#include "Core.h"
#include "Platform.h"
#include "Service.h"
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
    /// <summary>
    /// 创建主窗口
    /// </summary>
    /// <param name="props">窗口属性配置</param>
    /// <returns>创建成功返回 true，失败返回 false</returns>
    bool CreateMainWindow(const WindowProps& props = WindowProps());
    /// <summary>
    /// 销毁主窗口
    /// </summary>
    /// <returns>销毁成功返回 true，失败返回 false</returns>
    bool DestroyMainWindow();
    /// <summary>
    /// 创建子窗口
    /// </summary>
    /// <param name="props">窗口属性配置</param>
    /// <returns>创建成功返回 true，失败返回 false</returns>
    bool CreateSubWindow(const String& name, const WindowProps& props = WindowProps());
    /// <summary>
    /// 销毁子窗口
    /// </summary>
    /// <returns>销毁成功返回 true，失败返回 false</returns>
    bool DestroySubWindow(const String& name);
    /// <summary>
    /// 根据窗口名称获取对应的窗口指针
    /// </summary>
    /// <param name="name">窗口名称</param>
    /// <returns>找到则返回 Window*，否则返回 nullptr</returns>
    const Window* GetWindow(const String& name);
    /// <summary>
    /// 获取当前管理的窗口总数
    /// </summary>
    /// <returns>窗口数量</returns>
    int64 GetWindowCount() const;
    /// <summary>
    /// 销毁所有窗口
    /// </summary>
    void DestroyAllWindows();
private:
    /// <summary>
    /// 延迟销毁窗口
    /// 将窗口标记为待销毁，实际销毁操作在_ProcessDelayDestroy中执行
    /// </summary>
    /// <param name="name">要销毁的窗口名称</param>
    void _DelayDestroy(const String& name);
    /// <summary>
    /// 处理延迟销毁的窗口
    /// 在每帧Tick时调用，执行实际的窗口销毁操作
    /// </summary>
    void _ProcessDelayDestroy();
private:
    /// <summary>
    /// 主窗口实例，使用 ScopePtr 管理生命周期
    /// </summary>
    ScopePtr<Window> m_mainWindow;
    /// <summary>
    /// 子窗口容器，使用名称作为键值进行管理
    /// </summary>
    Map<String, ScopePtr<Window>> m_subWindows;
    /// <summary>
    /// 延迟销毁队列
    /// 存储待销毁的窗口名称，窗口的实际销毁会在下一帧执行
    /// 使用延迟销毁避免在当前帧操作过程中破坏窗口状态
    /// </summary>
    Array<String> m_delayDestory;
};