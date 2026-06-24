target "Sandbox"
    -- 设置目标编译类型：应用程序
    set_kind "binary"
    -- 设置目标文件生成目录
    set_targetdir "$(projectdir)/Sandbox/Build/$(os)/$(mode)/Bin"
    -- 设置中间文件生成目录
    set_objectdir "$(projectdir)/Sandbox/Build/$(os)/$(mode)/Obj"
    -- 设置依赖文件生成目录
    set_dependir "$(projectdir)/Sandbox/Build/$(os)/$(mode)/Dep"
    -- 添加头文件搜索目录
    add_includedirs "$(projectdir)/Engine/Source"
    -- 添加目标依赖
    add_deps "Engine"
    -- 添加链接搜索目录
    add_linkdirs("$(projectdir)/Engine/Build/$(os)/$(mode)/Bin")
    -- 指定编译配置
    add_rules("mode.Debug", "mode.Development", "mode.Test", "mode.Release")
    -- 添加链接库
    add_links("Engine")
    -- 添加程序运行时动态库加载路径
    add_runenvs("PATH","$(projectdir)/Engine/Build/$(os)/$(mode)/Bin")
    local vulkan_sdk = "$(projectdir)/ThirdParty/packages/v/vulkan/1.4.335/Build"
    add_runenvs("VULKAN_SDK", vulkan_sdk)
    add_runenvs("VK_LAYER_PATH", path.join(vulkan_sdk,"Bin"))
    -- 设置运行目录
    set_rundir"$(projectdir)/Sandbox"
    -- 设置运行参数列表
    set_runargs("-type=Game", "-pause")
    -- 添加头文件
    add_headerfiles{
        "Source/*.h"
    }
    -- 添加源代码文件
    add_files{
        "Source/*.cpp"
    }

    if is_plat("windows") then
        add_defines "PLATFORM_WINDOWS"
        add_syslinks("kernel32")  -- 链接 Windows API 库
        add_ldflags("/SUBSYSTEM:WINDOWS") -- 指定窗口子系统   
    end

    -- 构建完成后执行拷贝
    after_build(function(target)
        -- 定义源目录和目标目录（相对于项目根目录）
        local config_dir = path.join(os.projectdir(), "Engine/Config")
        local saved_dir  = path.join(os.scriptdir(), "Config/Engine")

        -- 确保目标目录存在
        os.mkdir(saved_dir)

        -- 递归拷贝 Config 下所有文件到 Saved
        os.cp(path.join(config_dir, "**"), saved_dir)
    end)
