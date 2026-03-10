#include "pch.h"

#include "Core.h"
#include "Platform.h"
#include "Runtime.h"
#include "String/String.h"
#include "String/Convert.h"
#include "String/TString.h"

#include <shellapi.h>

extern int32 EngineMain();

/// <summary>
/// 设置 Windows 环境
/// <para>CRT 相关设置</para>
/// </summary>
void SetWindowsEnvironment()
{

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

    // 命令行解析
    ProcessWindowsCommandLine();   
 
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