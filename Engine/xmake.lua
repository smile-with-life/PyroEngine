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
    -- 预编译头文件
    add_headerfiles("Source/pch.h")
    -- 添加预编译文件(msvc专用)
    add_files("Source/pch.cpp")
    -- 添加头文件搜索目录
    add_includedirs {
        "Source",
        "Source/Platform",
        "Source/Core",
        "Source/Module",
        "Source/Service",
        "Source/Application",
        "Source/Launch"
    }
    -- Windows设置
    if is_plat("windows") then
        -- 添加宏定义
        add_defines "PLATFORM_WINDOWS"
        -- 链接 Windows API 库
        add_syslinks("kernel32","User32", "shell32.lib")
        
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

    -- 构建配置加载
    on_load(function(target)
        local files = os.files(path.join(os.scriptdir(),"Source/**.lua"))
        for _, file in ipairs(files) do
            local dir = path.directory(file)
            local build = import("build",{ rootdir = dir })
            if build.config then
                local config = build.config()
                if config.headers then
                    for _, file in ipairs(config.headers) do
                        target:add("headerfiles", path.join(os.scriptdir(), file))
                    end
                end
                if config.files then
                    for _, file in ipairs(config.files) do
                        target:add("files", path.join(os.scriptdir(), file))
                    end
                end
            end
        end
    end)
    
    
    