target "Engine"
    -- 设置目标编译类型：动态库程序
    set_kind "shared"
    -- 设置代码语言标准：最新
    set_languages "cxxlatest"
    -- 设置目标文件生成目录
    set_targetdir "$(projectdir)/Engine/Build/$(os)/$(mode)/Bin"
    -- 设置中间文件生成目录
    set_objectdir "$(projectdir)/Engine/Build/$(os)/$(mode)/Obj"
    -- 设置依赖文件生成目录
    set_dependir "$(projectdir)/Engine/Build/$(os)/$(mode)/Dep"
    set_plat(os.host())
    set_arch(os.arch())
    -- 指定编译配置
    add_rules("mode.Test", "mode.Debug", "mode.Release", "mode.Dist")

    add_includedirs "Source"
    set_pcxxheader "Source/pch.h"

    add_syslinks "User32"

    add_defines "BUILD_EXPORT_DLL"

    add_rules("utils.symbols.export_all")
    

    if is_mode("release") then
        set_runtimes("MD")  -- Release 模式用动态 CRT
    elseif is_mode("debug") then
        set_runtimes("MD") -- Debug 模式用调试版动态 CRT
    end

    if is_os("windows") then
        add_defines "PLATFORM_WINDOWS"
        add_syslinks("kernel32")  -- 链接 Windows API 库
        -- add_rules("utils.symbols.export_list", {symbols = "WinMain"})
    end

    if is_os("linux") then
        add_defines {"PLATFORM_LINUX"}
    end

    if is_os("android") then
        add_defines {"PLATFORM_ANDROID"}
    end

    if is_os("macosx") then
        add_defines {"PLATFORM_MAC"}
    end

    if is_os("ios") then
        add_defines {"PLATFORM_IOS"}
    end

    

    add_headerfiles{
        "Source/pch.h"
    }
    add_files{
        "Source/pch.cpp"
    }

    -- 平台层
    add_headerfiles{
        "Source/Platform/Platform.h"
    }
    add_files{
        
    }
    if is_os("windows") then
        add_headerfiles{
            "Source/Platform/Windows/WindowsPlatform.h",
            "Source/Platform/Windows/WindowsConsole.h"
        }
        add_files{
            "Source/Platform/Windows/WindowsPlatform.cpp",
            "Source/Platform/Windows/WindowsConsole.cpp"
        }     
    end
    -- 核心层
    add_headerfiles{
        "Source/Core/Core.h"
    }
    add_files{
        
    }
    -- 功能层
    add_headerfiles{
        "Source/Function/Console/Console.h"
    }
    add_files{
        "Source/Function/Console/Console.cpp"
    }
    -- 服务层
    add_headerfiles{

    }
    add_files{

    }
    -- 应用层
    add_headerfiles{
        "Source/Application/Application.h"
    }
    add_files{
        "Source/Application/Application.cpp"
    }
    -- 入口点
    if is_os("windows") then
        add_headerfiles{
            
        }
        add_files{
            "Source/Launch/Windows/WindowsLaunch.cpp",
            "Source/Launch/Windows/WindowsExport.def"
        }     
    end
