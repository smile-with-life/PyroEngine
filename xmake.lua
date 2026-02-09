-- 设置工程名
set_project "PyroEngine"
-- 设置工程版本
set_version "0.1.0"
-- 设置编码
set_encodings "utf-8"
-- 设置最小 xmake 版本为：3.0.0
set_xmakever "3.0.0"
-- 设置允许编译的平台列表
set_allowedplats("windows", "linux", "android", "macosx", "ios")
-- 设置允许编译的平台架构
set_allowedarchs("x64")
-- 设置代码语言标准：最新
set_languages "cxxlatest"

rule("mode.Debug")
    on_config(function(target)      
        if is_mode("Debug") then
            -- 设置符号信息：添加调试符号
            target:set("symbols", "debug")
            -- 设置优化级别：禁用优化
            target:set("optimize", "none")
            -- 设置警告级别：启用全部支持的警告
            target:set("warnings", "everything")
        end  
    end)

rule("mode.Development")
    on_config(function(target)
        if is_mode("Development") then
            -- 设置符号信息：添加调试符号
            target:set("symbols", "debug")
            -- 设置优化级别：快速优化
            target:set("optimize", "fast")
            -- 设置警告级别：启用全部支持的警告
            target:set("warnings", "everything")
        end
    end)

rule("mode.Test")
    on_config(function(target)
        if is_mode("Test") then
            -- 设置符号信息：添加调试符号
            target:set("symbols", "debug")
            -- 设置优化级别：最快运行速度的优化	
            target:set("optimize", "fastest")
            -- 设置警告级别：启用全部支持的警告
            target:set("warnings", "less")
        end
    end)

rule("mode.Release")
    on_config(function(target)     
        if is_mode("Release") then
            -- 设置符号信息：设置符号不可见
            target:set("symbols", "hidden")
            -- 设置优化级别：最快运行速度的优化	
            target:set("optimize", "fastest")
            -- 剥夺调试符号信息：剥夺掉所有符号
            target:set("strip",  "all")
            -- 设置警告级别：禁用所有警告
            target:set("warnings", "none")
        end
    end)

-- 包含引擎工程描述文件
includes "Engine/xmake.lua"
includes "Sandbox/xmake.lua"