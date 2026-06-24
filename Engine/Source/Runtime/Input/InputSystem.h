#pragma once

#include "Core.h"
#include "Input.h"
#include "Container/Array.h"
#include "Container/HashMap.h"


class IGenericInputHandler
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~IGenericInputHandler() = default;
public:
    /// <summary>
    /// 处理输入
    /// </summary>
    /// <param name="input"></param>
    /// <returns>返回 true 时，停止传递</returns>
    virtual bool HandleInput(const InputData& input) = 0;
    /// <summary>
    /// 优先级：数字越大，越先处理
    /// </summary>
    /// <returns></returns>
    virtual uint32 GetPriority() const = 0;
};

/// <summary>
/// 输入系统
/// </summary>
class InputSystem
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    ~InputSystem() = default;
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
    void AddInputHandler(uint64 id, IGenericInputHandler* handler);

    void RemoveInputHandler(uint64 id, IGenericInputHandler* handler);

    void DeferredProcessInput(uint64 id, const InputData& input);

    void DispatchInput(uint64 id, const InputData& input);

    void ClearChannel(uint64 id);

    void ClearAll();

    bool Contains(uint64 id);

    bool Contains(uint64 id, IGenericInputHandler* handler);
public:
    /// <summary>
    /// 获取输入系统单例实例
    /// </summary>
    /// <returns>输入系统实例的引用</returns>
    static InputSystem& GetInstance();
protected:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    InputSystem() = default;
private:
    void _SortByPriority(Array<IGenericInputHandler*>& inputHandlers);
private:
    /// <summary>
    /// 多通道输入处理器
    /// </summary>
    HashMap<uint64, Array<IGenericInputHandler*>> m_inputHandlePaths;
    /// <summary>
    /// 延迟输入队列
    /// </summary>
    Array<std::pair<uint64, InputData>> m_deferredInputDatas;
};