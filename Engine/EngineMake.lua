project "Engine" --项目
    kind "SharedLib" --生成类型
    language "C++" --语言
    cppdialect "C++23" --C++标准
    pchheader "pch.h" --预编译头文件
    pchsource "Source/pch.cpp" --VS下的预编译文件

    -- 变量定义
    local outputDir = "Build/" .. SystemName[_TARGET_OS] .. "/%{cfg.buildcfg}" -- 输出目录

    EngineSourcePath = WorkPath .. EnginePath .. "Source/" -- 引擎源码路径
    EngineLibPath = WorkPath .. EnginePath .. "Build/" .. SystemName[_TARGET_OS] .. "/%{cfg.buildcfg}/Bin"

    targetdir (outputDir .. "/Bin") --引擎库文件目录
    objdir (outputDir .. "/Obj") --中间文件目录

    files --将文件添加到项目中
    {
        "Source/pch.h",
        "Source/pch.cpp",
    }

    includedirs --指定文件搜索路径
    {
        "Source",
    }

    libdirs --库搜索路径
    {

    }

    links --链接库
    {

    }

    include "Source/Platform/PlatformMake.lua" --平台层
    include "Source/Core/CoreMake.lua" --核心层
    include "Source/Function/FunctionMake.lua" --功能层
    include "Source/Server/ServerMake.lua" --服务层
    include "Source/Application/ApplicationMake.lua" --应用程序层
    include "Source/Launch/LaunchMake.lua" --入口层

    filter "configurations:Test" --Test模式
	    defines "BUILD_CONFIG_TEST"
		runtime "Debug"
		symbols "On"

    filter "configurations:Debug" --Debug模式
	    defines "BUILD_CONFIG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release" --Release模式
	    defines "BUILD_CONFIG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist" --Dist模式
	    defines "BUILD_CONFIG_DIST"
		runtime "Release"
		optimize "Full"

    filter "system:Windows" --当系统是Windows时
		staticruntime "On"
		systemversion "latest"

		defines --预定义宏
		{
			"PLATFORM_WINDOWS",
			"BUILD_EXPORT_DLL"
		}
