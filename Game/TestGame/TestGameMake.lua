target "TestGame"
    -- 设置目标编译类型：应用程序
    set_kind "binary"
    -- 设置代码语言标准：最新
    set_languages "cxxlatest"
    -- 设置目标文件生成目录
    set_targetdir "$(projectdir)/Game/TestGame/Build/$(os)/$(mode)/Bin"
    -- 设置中间文件生成目录
    set_objectdir "$(projectdir)/Game/TestGame/Build/$(os)/$(mode)/Obj"
    -- 设置依赖文件生成目录
    set_dependir "$(projectdir)/Game/TestGame/Build/$(os)/$(mode)/Dep"
    -- 添加头文件搜索目录
    add_includedirs "$(projectdir)/Engine/Source"
    -- 指定编译配置
    add_rules("mode.Test", "mode.Debug", "mode.Release", "mode.Dist")
    -- 添加目标依赖
    add_deps "Engine"
    -- 添加链接搜索目录
    add_linkdirs("$(projectdir)/Engine/Build/$(os)/$(mode)/Bin")
    -- 添加链接库
    add_links("Engine")
    -- 添加程序运行时动态库加载路径
    add_runenvs("PATH","$(projectdir)/Engine/Build/$(os)/$(mode)/Bin")
    -- 设置运行目录
    set_rundir"$(projectdir)/Game/TestGame"

    if is_os("windows") then
        add_defines "PLATFORM_WINDOWS"
        add_syslinks("kernel32")  -- 链接 Windows API 库
        add_ldflags("/SUBSYSTEM:WINDOWS") -- 指定窗口子系统
        -- set_runtimes "MDd"
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


    add_files{
        "Source/*.cpp"
    }

    add_headerfiles{
        "Source/*.h"
    }