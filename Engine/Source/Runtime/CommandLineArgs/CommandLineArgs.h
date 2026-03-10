#pragma once

#include "Core.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Container/Map.h"

/// <summary>
/// 命令行参数
/// </summary>
class CommandLineArgs
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    ~CommandLineArgs() = default;
public:
    /// <summary>
    /// 从 C 风格的 argc/argv 解析
    /// </summary>
    /// <param name="argc">参数个数</param>
    /// <param name="argv">参数数组</param>
    bool Parse(int32 argc, char** argv);
    /// <summary>
    /// 判断是否包含指定参数
    /// </summary>
    /// <param name="args">参数名（可带 - 或 / 前缀）</param>
    /// <returns>如果包含返回 true</returns>
    bool HasArg(const String& arg) const;
    /// <summary>
    /// 获取指定参数的值
    /// </summary>
    /// <param name="args">参数名（可带 - 或 / 前缀）</param>
    /// <returns>参数值，如果不存在返回空字符串</returns>
    String GetArgValue(const String& arg) const;
    /// <summary>
    /// 获取所有参数的键名列表
    /// </summary>
    Array<String> GetArgs() const;
    /// <summary>
    /// 获取参数数量
    /// </summary>
    /// <returns>有效命令数量</returns>
    int32 Count() const;
    /// <summary>
    /// 获取程序路径
    /// </summary>
    /// <returns>可执行文件路径</returns>
    String ExePath() const;
    /// <summary>
    /// 清空所有参数
    /// </summary>
    void Clear();
public:
    /// <summary>
    /// 获取命令行参数单例实例
    /// </summary>
    /// <returns></returns>
    static CommandLineArgs& GetInstance();
protected:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    CommandLineArgs() = default;
private:
    /// <summary>
    /// 规范化参数名（去除前缀并转换为小写）
    /// </summary>
    String _NormalizeKey(const String& key) const;
private:
    /// <summary>
    /// 参数数据
    /// 键为参数名（不带 - 或 / 前缀）
    /// 值为参数值（如果是开关参数则为空字符串）
    /// </summary>
    Map<String, String> m_data;
    /// <summary>
    /// 程序路径
    /// </summary>
    String m_exePath;
};