#pragma once

#include "Core.h"
#include "String/String.h"
#include "Json/Json.h"
#include "Config/Config.h"

class ConfigFile
{
public:
    using iterator = typename Map<ConfigKey, Config>::iterator;
    using const_iterator = typename Map<ConfigKey, Config>::const_iterator;
    using reverse_iterator = typename Map<ConfigKey, Config>::reverse_iterator;
    using const_reverse_iterator = typename Map<ConfigKey, Config>::const_reverse_iterator;
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    ConfigFile() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~ConfigFile() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    ConfigFile(const ConfigFile& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    ConfigFile& operator=(const ConfigFile& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    ConfigFile(ConfigFile&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    ConfigFile& operator=(ConfigFile&& other) = default;
    /// <summary>
    /// 构造函数，从 Json 构造
    /// </summary>
    /// <param name="value"></param>
    explicit ConfigFile(const Json& json);
public:
    bool IsValid() const;
public:
    Config operator[](const String& key) const;
public:
    /// <summary>
    /// 返回指向第一个元素的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return m_data.begin();
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }
    /// <summary>
    /// 返回指向第一个元素的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }
    /// <summary>
    /// 返回指向末尾的迭代器
    /// </summary>
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_data.end();
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_data.end();
    }
    /// <summary>
    /// 返回指向末尾的const迭代器
    /// </summary>
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return m_data.cend();
    }
    /// <summary>
    /// 返回指向最后一个元素的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return m_data.rbegin();
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return m_data.rbegin();
    }
    /// <summary>
    /// 返回指向最后一个元素的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return m_data.crbegin();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的反向迭代器
    /// </summary>
    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return m_data.rend();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return m_data.rend();
    }
    /// <summary>
    /// 返回指向第一个元素前一个位置的const反向迭代器
    /// </summary>
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return m_data.crend();
    }
private:
    void _ParseConfig(const Json& json);
private:
    /// <summary>
    /// 配置名
    /// </summary>
    String m_name;
    /// <summary>
    /// 配置版本
    /// </summary>
    String m_version;
    /// <summary>
    /// 配置生成者
    /// </summary>
    String m_generator;
    /// <summary>
    /// 配置数据
    /// </summary>
    Map<ConfigKey, Config> m_data;
};