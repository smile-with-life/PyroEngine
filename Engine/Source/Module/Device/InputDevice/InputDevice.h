#pragma once
#include "Core.h"
#include "Device/Device.h"

class InputDevice : public Device
{
public:
    InputDevice() = default;

    ~InputDevice() = default;

    InputDevice(const InputDevice& other) = delete;

    InputDevice& operator=(const InputDevice& other) = delete;

    InputDevice(InputDevice&& other) = default;

    InputDevice& operator=(InputDevice&& other) = default;
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