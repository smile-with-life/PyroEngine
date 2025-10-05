--includes "Dependencies/PackageMake.lua"

--add_requires("GoogleTest", {configs = {shared = false}})  -- 静态链接

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
    -- 指定编译配置
    add_rules("mode.Test", "mode.Debug", "mode.Release", "mode.Dist")
    -- 自动链接gtest库
    --add_packages("GoogleTest")  
    -- 添加头文件搜索目录
    add_includedirs {
        "Source",
        "Source/Core",
        "Source/Platform",
        "Source/Function",
        "Source/Server",
        "Source/Application"
    }
    -- 设置 C++ 预编译头文件
    set_pcxxheader "Source/pch.h"
    -- 自动导出所有的动态库符号
    add_rules("utils.symbols.export_all")
    -- 动态库设置
    add_defines "BUILD_EXPORT_DLL"
    -- Windows设置
    if is_plat("windows") then
        -- 添加宏定义
        add_defines "PLATFORM_WINDOWS"
        -- 链接 Windows API 库
        add_syslinks("kernel32","User32")
        -- 添加头文件
        add_headerfiles{
            -- 平台层：Windows平台
            "Source/Platform/Windows/**.h",
        }
        -- 添加源代码文件
        add_files{
            -- 平台层：Windows平台
            "Source/Platform/Windows/**.cpp",
            -- Windows入口
            "Source/Launch/Windows/WindowsLaunch.cpp",
            "Source/Launch/Windows/WindowsExport.def"
        }     
        -- Test配置
        if is_mode("Test") then
            add_defines "BUILD_CONFIG_TEST=0"
            set_runtimes "MDd"
        end
        -- Debug配置
        if is_mode("Debug") then
            add_defines "BUILD_CONFIG_DEBUG=1"
            set_runtimes "MDd"
        end
        -- Release配置
        if is_mode("Release") then
            add_defines "BUILD_CONFIG_RELEASE=2"
            set_runtimes "MD"
        end
        -- Dist配置
        if is_mode("Dist") then
            add_defines "BUILD_CONFIG_DIST=3"
            set_runtimes "MD"
        end 
    end
    -- Linux设置
    if is_plat("linux") then
        add_defines {"PLATFORM_LINUX"}
    end
    -- Android设置
    if is_plat("android") then
        add_defines {"PLATFORM_ANDROID"}
    end
    -- Mac设置
    if is_plat("macosx") then
        add_defines {"PLATFORM_MAC"}
    end
    -- IOS设置
    if is_plat("ios") then
        add_defines {"PLATFORM_IOS"}
    end    

    add_headerfiles{
        -- 预编译头文件
        "Source/pch.h",
        -- 核心层
        "Source/Core/**.h",
        -- 平台层
        "Source/Platform/*.h",
        -- 功能层
        "Source/Function/**.h",
        -- 服务层
        "Source/Server/**.h",
        -- 应用层
        "Source/Application/**.h"
    }
    add_files{
        -- 预编译文件(msvc专用)
        "Source/pch.cpp",
        -- 核心层
        "Source/Core/**.cpp",
        -- 平台层
        "Source/Platform/**.cpp",
        -- 功能层
        "Source/Function/**.cpp",
        -- 服务层
        "Source/Server/**.cpp",
        -- 应用层
        "Source/Application/**.cpp"
    }



