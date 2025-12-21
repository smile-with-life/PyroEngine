#include "pch.h"

#include "Core.h"
#include "Platform.h"
#include "Application.h"

// 设置 Windows 环境 

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
    return false;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{ 
    // @性能追踪标记
    
    // @启用内存错误检测工具 AddressSanitizer

    // 设置 Windows 环境
    SetWindowsEnvironment();

    int32 error = 0;

    // 设置 Windows 应用程序句柄
    GWindowsInstance = hInstance;

    // 命令行解析
    tchar* cmdLine = GetCommandLineW();

    // 获取应用程序单例
    Application& app = Application::GetInstance();
#ifdef PLATFORM_SEH_EXCEPTIONS
    __try
#endif
    {
        error = app.Run();
    }
#ifdef PLATFORM_SEH_EXCEPTIONS
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        // 处理 SEH 异常
    }
#endif
    return error;
}