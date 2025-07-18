workspace "PyroEngine" --工作区
    system "Windows"
	architecture "x86_64" --架构
    characterset "Unicode" --字符集
    startproject "TestGame" --[[启动项目]]

	configurations --配置项
    {    
        "Test",                        
        "Debug",
        "Release",
        "Dist"
    }

    flags  --设置编译器选项
	{
		"MultiProcessorCompile" --多处理器并行编译
	}

outputDir = "%{prj.name}/Build/%{cfg.system}/{cfg.buildcfg}" --输出目录

include "Engine/Engine/EngineMake.lua" -- 引擎构建
include "Game/TestGame/TestGameMake.lua" --游戏构建    
    
    