#include "pch.h"

#include "Core.h"
#include "Platform.h"
#include "Runtime.h"
#include "String/String.h"
#include "String/Convert.h"
#include "String/TString.h"
#include "CommandLineArgs/CommandLineArgs.h"
#include "Logger/Logger.h"

#include <shellapi.h>

extern int32 EngineMain();

// 显卡切换策略
// 在拥有双显卡（如集成+独立）的笔记本电脑上，引导系统使用高性能的 NVIDIA 或 AMD 独立显卡来运行本程序
extern "C" { _declspec(dllexport) uint32 NvOptimusEnablement = 0x00000001; }
extern "C" { _declspec(dllexport) uint32 AmdPowerXpressRequestHighPerformance = 0x00000001; }

/// <summary>
/// CRT 参数校验错误处理函数
/// </summary>
/// <param name="Expression">触发非法的表达式</param>
/// <param name="Function">出错的函数名</param>
/// <param name="File">出错的文件名</param>
/// <param name="Line">出错的行号</param>
/// <param name="Reserved">保留参数（不使用）</param>
void InvalidParamHandler(const tchar* Expression, const tchar* Function, const tchar* File, uint32 Line, uintptr_t Reserved)
{
    String expression = Convert::ToString(TString(Expression));
    String function = Convert::ToString(TString(Function));
    String file = Convert::ToString(TString(File));
    GLog->Fatal("\nCRT: Invalid Param:\n -Expression: {}\n -Function: {}\n -File: {}\n -Line: {}",
        expression.IsEmpty() ? "Unknown" : expression,
        function.IsEmpty() ? "Unknown" : function,
        file.IsEmpty() ? "Unknown" : file,
        Line);
}

/// <summary>
/// 设置 Windows 环境
/// <para>CRT 相关设置</para>
/// </summary>
void SetWindowsEnvironment()
{
    // 所有 CRT（C 运行时）的参数错误、非法调用，都执行自定义的回调函数
    // 而不是弹出系统默认的“应用程序已停止工作”弹窗
    _set_invalid_parameter_handler(InvalidParamHandler);

    // 判断是否启用调试功能
    if (GIsDebugMode)
    {
        // 关闭 CRT 断言的弹窗，直接输出到日志
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

        // 关闭 CRT 调试模式下的“内存填充”机制（默认会把释放的内存填 0xFD 等）
        // @ 注释-不知道会不会用到
        // _CrtSetDebugFillThreshold(0);
    }
}

/// <summary>
/// 解析 Windows 命令行参数
/// </summary>
bool ProcessWindowsCommandLine()
{
    // 获取完整的命令行字符串
    tchar* cmdLine = GetCommandLineW();

    if (!cmdLine)
    {
        return false;
    }

    // 跳过程序路径（第一个参数）
    int argc = 0;
    tchar** targv = nullptr;

    // 使用 Windows API 将命令行分解为标准的 argv/argc 格式
    // 这会正确处理带引号的参数和转义字符
    targv = CommandLineToArgvW(cmdLine, &argc);

    if (!targv)
    {
        return false;
    }

    // 转换为 UTF-8 字符串数组
    Array<String> args;
    for (int32 i = 0; i < argc; ++i)
    {
        TString nativeString(targv[i]);
        String str = Convert::ToString(nativeString);
        args.Add(str);
    }

    // 构建 char* 数组供 CommandLineArgs::Parse 使用
    Array<const char*> argPtrs;
    for (const auto& arg : args)
    {
        argPtrs.Add(arg.ToCString());
    }
    char** argv = const_cast<char**>(argPtrs.Data());

    // 调用 CommandLineArgs 的 Parse 方法
    bool result = GCommandLineArgs->Parse(argc, argv);

    // 释放 CommandLineToArgvW 分配的内存
    LocalFree(targv);

    return result;
}

/// <summary>
/// 处理 SEH 异常的包装函数
/// </summary>
int32 EngineMainWrapper()
{
    __try
    {
        return EngineMain();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        DWORD code = GetExceptionCode();

        // @ 实现 错误处理

        return static_cast<int32>(code);
    }
}

/// <summary>
/// Windows 应用程序的入口点。该函数在 CRT 启动代码完成后被调用，
/// 负责设置引擎运行环境，并最终调用引擎主函数 EngineMain()
/// </summary>
/// <param name="hInstance">当前应用程序实例的句柄</param>
/// <param name="hPrevInstance">前一个实例的句柄。在 Win32 环境下始终为 NULL</param>
/// <param name="lpCmdLine">命令行字符串</param>
/// <param name="nShowCmd">指定窗口如何显示的标志</param>
/// <returns>返回程序退出码</returns>
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    int32 error = 0;

    // 设置 Windows 环境
    SetWindowsEnvironment();

    // 将平台命令行参数解析为引擎通用命令行参数
    ProcessWindowsCommandLine();   
 
    // 解析命令行参数判断是否支持在退出前暂停程序
    bool shouldPauseBeforeExit = false;
    if (GCommandLineArgs->HasArg("-pause"))
    {
        shouldPauseBeforeExit = true;
    }

    // 设置 Windows 应用程序句柄
    GWindowsInstance = hInstance;

    // 根据平台宏选择异常处理方式
#ifdef PLATFORM_SEH_EXCEPTIONS
    error = EngineMainWrapper();
#else
    error = EngineMain();
#endif
    
    // 控制是否在退出前暂停进程
    if (shouldPauseBeforeExit)
    {
        Sleep(INFINITE);
    }

    return error;
}