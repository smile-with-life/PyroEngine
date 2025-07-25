workspace "PyroEngine" --工作区
    system "Windows"
	architecture "x86_64" --架构
    characterset "Unicode" --字符集
    startproject "TestGame" --[[启动项目]]

    -- 变量定义
    SystemName = {
        windows = "Windows",
        linux = "Linux"
    }
    WorkPath = os.getcwd() .. "/" -- 工作目录
    EnginePath = "Engine/" -- 游戏引擎目录
    GamePath = "Game/" -- 游戏目录

	configurations --构建配置
    {    
        "Test",                        
        "Debug",
        "Release",
        "Dist"
    }

    flags  --设置编译器选项
	{
		"MultiProcessorCompile" --启用多核编译
	}

include "Engine/EngineMake.lua" -- 引擎构建
include "Game/TestGame/TestGameMake.lua" --游戏构建    
    
    