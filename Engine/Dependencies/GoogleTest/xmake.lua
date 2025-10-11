target "GoogleTest"
    -- 设置目标编译类型：静态库程序
    set_kind "static"
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
    -- 平台特定配置
    if is_plat("linux", "macosx") then
        add_links("pthread")
        add_defines("GTEST_HAS_PTHREAD=1")
    elseif is_plat("windows") then
        add_defines("_CRT_SECURE_NO_WARNINGS")
    end
    -- 添加头文件搜索目录
    add_includedirs {
        "include",
        "."
    }
    --
    add_headerfiles{      
        "include/**.h"     
    }
    -- 
    add_files(
        "src/**.cc"
    )