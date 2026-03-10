#include "pch.h"

#include "CommandLineArgs.h"

/* ==================== static ==================== */
CommandLineArgs& CommandLineArgs::GetInstance()
{
    static CommandLineArgs instance;
    return instance;
}

/* ==================== public ==================== */
bool CommandLineArgs::Parse(int32 argc, char** argv)
{
    // 清空现有数据
    m_data.Clear();

    if (argc < 1 || argv == nullptr)
    {
        return false;
    }

    // 保存程序路径
    if (argv[0] != nullptr)
    {
        m_exePath = String(argv[0]);
    }

    // 从索引1开始遍历，跳过程序路径
    for (int32 i = 1; i < argc; ++i)
    {
        if (argv[i] == nullptr)
        {
            continue;
        }

        String arg(argv[i]);

        // 检查是否为参数（以 - 或 / 开头）
        if (arg.StartWith("-") || arg.StartWith("/"))
        {
            // 处理 -key=value 格式
            int64 pos = arg.IndexOf('=');
            if (pos != -1)
            {
                String key = arg.SubStr(1, pos - 1);
                String value = arg.SubStr(pos + 1, arg.Count() - pos - 1);

                // 移除值可能带有的引号
                if (value.StartWith("\"") && value.EndWith("\""))
                {
                    value = value.SubStr(1, value.Count() - 2);
                }

                m_data.Add({ _NormalizeKey(key), value });
            }
            else
            {
                // 处理开关参数（无值）
                String key = arg.SubStr(1, arg.Count() - 1);

                // 检查下一个参数是否是值（-key value 格式）
                if (i + 1 < argc && argv[i + 1] != nullptr)
                {
                    String nextArg(argv[i + 1]);
                    if (!nextArg.StartWith("-") && !nextArg.StartWith("/"))
                    {
                        // -key value 格式：将下一个参数作为当前参数的值
                        m_data.Add({ _NormalizeKey(key), nextArg });
                        ++i; // 跳过已处理的下一个参数
                    }
                    else
                    {
                        // 纯开关参数（无值）
                        m_data.Add({ _NormalizeKey(key), String() });
                    }
                }
                else
                {
                    // 最后一个参数且无值，纯开关参数
                    m_data.Add({ _NormalizeKey(key), String() });
                }
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool CommandLineArgs::HasArg(const String& arg) const
{
    String key = _NormalizeKey(arg);
    return m_data.Contains(key);
}

String CommandLineArgs::GetArgValue(const String& arg) const
{
    String key = _NormalizeKey(arg);
    return m_data.At(key);
}

Array<String> CommandLineArgs::GetArgs() const
{
    Array<String> result;

    for (const auto& pair : m_data)
    {
        result.Add(pair.first);
    }

    return result;
}

int32 CommandLineArgs::Count() const
{
    return m_data.Size();
}

String CommandLineArgs::ExePath() const
{
    return m_exePath;
}

void CommandLineArgs::Clear()
{
    m_data.Clear();
}

/* ==================== private ==================== */
String CommandLineArgs::_NormalizeKey(const String& key) const
{
    String normalized = key;

    // 去除可能残留的前缀
    while (normalized.StartWith("-") || normalized.StartWith("/"))
    {
        normalized = normalized.SubStr(1, normalized.Count() - 1);
    }

    // 转换为小写以便不区分大小写匹配
    for (int64 i = 0; i < normalized.Count(); ++i)
    {
        Char ch = normalized[i];
        if (ch.IsASCII() && ch.IsLetter())
        {
            normalized[i] = ch.ToLower();
        }
    }

    return normalized;
}
