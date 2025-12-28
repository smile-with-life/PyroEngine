-- 设置工程名
set_project "PyroEngine"
-- 设置工程版本
set_version "0.1.0"
-- 设置编码
set_encodings "utf-8"
-- 设置最小 xmake 版本为：3.0.0
set_xmakever "3.0.0"
-- 设置允许编译的平台列表
set_allowedplats("windows","linux","android","macosx","ios")
-- 设置允许编译的平台架构
set_allowedarchs("x64")
-- 设置代码语言标准：最新
set_languages "cxxlatest"


rule("mode.Debug")
    on_load(function(target)
        -- 设置符号信息：添加调试符号
        target:set("symbols", "debug")
        -- 设置优化级别：禁用优化
        target:set("optimize", "none")
        -- 设置警告级别：启用全部支持的警告
        target:set("warnings", "everything")
    end)

rule("mode.Development")
    on_load(function(target)
        -- 设置符号信息：添加调试符号
        target:set("symbols", "debug")
        -- 设置优化级别：禁用优化
        target:set("optimize", "none")
        -- 设置警告级别：启用全部支持的警告
        target:set("warnings", "everything")
    end)


rule("mode.Release")
    on_load(function(target)
        -- 设置符号信息：添加调试符号
        target:set("symbols", "debug")
        -- 设置优化级别：禁用优化
        target:set("optimize", "none")
        -- 设置警告级别：启用全部支持的警告
        target:set("warnings", "everything")
    end)

rule("mode.Test")
    on_load(function(target)
        -- 设置符号信息：添加调试符号
        target:set("symbols", "debug")
        -- 设置优化级别：禁用优化
        target:set("optimize", "none")
        -- 设置警告级别：启用全部支持的警告
        target:set("warnings", "everything")
    end)



-- 包含引擎工程描述文件
includes "Engine/EngineMake.lua"
-- 包含游戏工程描述文件
includes "Game/GameMake.lua"

