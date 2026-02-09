target "Engine"
    -- 设置目标编译类型：动态库程序
    set_kind "shared"
    -- 动态库设置
    add_defines "BUILD_EXPORT_DLL"
    -- 设置目标文件生成目录
    set_targetdir "$(projectdir)/Engine/Build/$(os)/$(mode)/Bin"
    -- 设置中间文件生成目录
    set_objectdir "$(projectdir)/Engine/Build/$(os)/$(mode)/Obj"
    -- 设置依赖文件生成目录
    set_dependir "$(projectdir)/Engine/Build/$(os)/$(mode)/Dep"
    -- 指定编译配置
    add_rules("mode.Debug", "mode.Development", "mode.Test", "mode.Release")
    -- 添加头文件搜索目录
    add_includedirs {
        "Source",
    }
    -- 添加头文件
    add_headerfiles{
    }
    -- 添加源代码文件
    add_files{
    }

    -- Windows设置
    if is_plat("windows") then
        -- 添加宏定义
        add_defines "PLATFORM_WINDOWS"
        -- 链接 Windows API 库
        add_syslinks("kernel32","User32")
        -- 添加头文件
        add_headerfiles{
        }
        -- 添加源代码文件
        add_files{
            -- Windows 入口
            "Source/Launch/Windows/WindowsLaunch.cpp",
            "Source/Launch/Windows/WindowsExport.def"
        }     
        -- Debug配置
        if is_mode("Debug") then
            add_defines "BUILD_CONFIG_DEBUG=0"
            set_runtimes "MDd"
        end
        -- Development配置
        if is_mode("Development") then
            add_defines "BUILD_CONFIG_DEVELOPMENT=1"
            set_runtimes "MDd"
        end
        -- Release配置
        if is_mode("Release") then
            add_defines "BUILD_CONFIG_RELEASE=2"
            set_runtimes "MD"
        end
        -- Test配置
        if is_mode("Test") then
            add_defines "BUILD_CONFIG_TEST=3"
            set_runtimes "MD"
        end   
    end