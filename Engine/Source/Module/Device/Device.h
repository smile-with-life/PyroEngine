#pragma once
#include "Core.h"
#include "String/String.h"

// 设备类型
enum class DeviceType
{
    GraphicsDevice,     // 图形设备
    AudioDevice,        // 音频设备
    InputDevice,        // 输入设备
    StroageDevice       // 存储设备
};

// 设备状态
enum class DeviceStatus
{
    None,               // 未知
    Normal,             // 正常
    Disabled,           // 禁用
    MissingDriver,      // 缺少驱动
    Error               // 硬件错误
};

// 设备信息
struct DeviceInfo
{
    /// <summary>
    /// 设备标识
    /// </summary>
    String Id;         
    /// <summary>
    /// 设备名称
    /// </summary>
    String Name;    
    /// <summary>
    /// 设备类型
    /// </summary>
    DeviceType Type;    
    /// <summary>
    /// 设备状态
    /// </summary>
    DeviceStatus Status;  
    /// <summary>
    /// 驱动版本
    /// </summary>
    String DriverVersion;
    /// <summary>
    /// 制造商
    /// </summary>
    String Manufacturer;
};

// 设备抽象类
class Device
{
public:
    Device() = default;

    ~Device() = default;

    Device(const Device& other) = delete;

    Device& operator=(const Device& other) = delete;

    Device(Device&& other) = default;

    Device& operator=(Device&& other) = default;
public:
    /// <summary>
    /// 获取设备标识
    /// </summary>
    /// <returns>设备标识</returns>
    String GetId() const;
    /// <summary>
    /// 获取设备名称
    /// </summary>
    /// <returns>设备名称</returns>
    String GetName() const;
    /// <summary>
    /// 获取设备类型
    /// </summary>
    /// <returns>设备类型</returns>
    DeviceType GetType() const;
    /// <summary>
    /// 获取设备状态
    /// </summary>
    /// <returns></returns>
    DeviceStatus GetDeviceStatus() const;
    /// <summary>
    /// 获取设备驱动版本
    /// </summary>
    /// <returns>驱动版本</returns>
    String GetDriverVersion() const;
    /// <summary>
    /// 获取设备制造商
    /// </summary>
    /// <returns>制造商</returns>
    String GetManufacturer() const;
    /// <summary>
    /// 获取设备信息
    /// </summary>
    /// <returns>设备信息</returns>
    DeviceInfo GetDeviceInfo() const;
};
